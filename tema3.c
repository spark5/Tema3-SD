/**
 *Oncioiu Anamaria Raluca, 313CA, Tema3, Data: 26.04.2011 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hash.h"


#define MAX 64
#define HMAX 16411


struct tab_disp { unsigned int max, n; List *H;};
typedef struct tab_disp *Hash; 

struct camera {char nume[MAX]; Hash obiecte;};
typedef struct camera *Room;

struct obiect {char nume[MAX]; unsigned int nr; int stare;};
typedef struct obiect *Object; 


/*functie care cauta intr-un vector de siruri de caractere, o valoare anume*/
int cauta(char **v, unsigned int n, char *value)
{
	unsigned int i;
	
	for(i = 0; i < n; i++)
		if(strcmp(v[i], value) == 0)
			return 1;
		
	return 0;
}


/*functia creeaza o tabela de dispersie deschisa cu numBuckets liste*/
Hash Hash_create(unsigned int numBuckets)
{
	unsigned int i;
	Hash h = (Hash )malloc(sizeof(struct tab_disp));
	
	h->max = numBuckets;
	h->n = 0;
	h->H = (List *)malloc(numBuckets * sizeof(List));
	
	for(i = 0; i < h->max; i++)
		h->H[i] = NULL;	
		
	return h;
}


/*functia de stergere a unui hashtable*/
void Hash_free(Hash *h)
{
	unsigned int i, j, k, l, n = 0, m = 0;
	Room p = NULL;
	
	for(i = 0; i < (*h)->max; i++)
		if(isEmpty((*h)->H[i]) == 1)
		{
			n = sizeOfList((*h)->H[i]);
		
			for(j = 0; j < n; j++)
			{
				p = (Room)elementAt((*h)->H[i], j);
		
				for(k = 0; k < p->obiecte->max; k++)
					if(isEmpty(p->obiecte->H[k]) == 1)
					{
						m = sizeOfList(p->obiecte->H[k]);
					
						for(l = 0; l < m; l++) 
							delAt(&p->obiecte->H[k], l);
						
						free(p->obiecte->H[k]);
					}
				
				free(p->obiecte);	
				delAt(&((*h)->H[i]), j);
			}			
		
			free((*h)->H[i]);
		}	
		
	(*h)->n = 0;
	(*h)->max = 0;
	free((*h)->H);
	free(*h);	
}


/*functie care cauta in hashtable o pereche key-value; 
 *daca o gaseste, updateaza continutul*/
void* Hash_search(Hash h, char *key, char *value, int nr, char simbol)
{
	int found = 0;
	unsigned int i, j, n = 0, m = 0, index, poz;
	Object obj;
	Room p;
	Object q;
	
	index = MurmurHash(key);
	
	if(isEmpty(h->H[index]) == 1)
		n = sizeOfList(h->H[index]);
	
	/*se cauta in lista cu elemente de tip Room*/
	for(i = 0; i < n; i++)
	{
		p = (Room)elementAt(h->H[index], i);
		
		/*daca s-a gasit camera intr-unul din nodurile listei*/
		if(strcmp(p->nume, key) == 0)
		{
			/*se calculeaza indexul listei de obiecte in hashtable-ul de obiecte*/
			poz = MurmurHash(value);
			
			/*daca exista lista*/
			if(isEmpty(p->obiecte->H[poz]) == 1)
			{
				m = sizeOfList(p->obiecte->H[poz]);
			
				/*se cauta in lista de obiecte*/
				for(j = 0; j < m; j++)
				{
					q = (Object)elementAt(p->obiecte->H[poz], i);
				
					/*daca s-a gasit obiectul*/
					if(strcmp(q->nume, value) == 0)
					{	
						found = 1;
					
						/*daca se precizeaza cantitatea exacta,
						 *starea obiectului trebuie sa fie 0;
						 *altfel, starea ramane nemodificata
						 */
						if(simbol == '=')
						{
							q->stare = 0;
							q->nr = nr;
						}
						else
							q->nr += nr;
						
						return value;
					}
				}
			
				/*daca nu s-a gasit obiectul*/
				if(found == 0)
				{
					/*se creste numarul de intrari in hashtable-ul de obiecte*/
					p->obiecte->n++;
					
					/*se aloca memorie pentru un element de tip Object*/
					obj = (Object)malloc(sizeof(struct obiect));
					strcpy(obj->nume, value);
					obj->nr = nr;
					
					/*daca se precizeaza cantitatea exacta,
					 *starea obiectului trebuie sa fie 0;
					 *altfel, starea ramane nemodificata
					 */
					if(simbol == '=')
						obj->stare = 0;
					else
						obj->stare = 1;
				
					/*se insereaza obiectul in lista de obiecte*/	
					insertAt(&p->obiecte->H[poz], obj, 1);
				
					return value;	
				}
			}
			else
				{
					/*lista nu exista, trebuie inserat obiectul*/
					
					/*se creste numarul de intrari in hashtable-ul de obiecte*/
					p->obiecte->n++;
					
					/*se aloca memorie pentru un element de tip Object*/
					obj = (Object)malloc(sizeof(struct obiect));
					strcpy(obj->nume, value);
					obj->nr = nr;
					
					/*daca se precizeaza cantitatea exacta,
					 *starea obiectului trebuie sa fie 0;
					 *altfel, starea ramane nemodificata
					 */
					if(simbol == '=')
						obj->stare = 0;
					else
						obj->stare = 1;
				
					/*se insereaza obiectul in lista de obiecte*/	
					insertAt(&p->obiecte->H[poz], obj, 1);
				
					return value;	
				}				
		}			
	}
			
	return 0;
}


/*functia insereaza in hashtable o pereche key-value, daca nu e deja inserata*/
void* Hash_insert(Hash h, char *key, char *value, int nr, char simbol)
{	
	unsigned int poz;
	Room r = NULL;
	Object obj = NULL;
	
	/*daca perechea key-value nu a fost inserata*/
	if(Hash_search(h, key, value, nr, simbol) != 0)
		return 0;	
	
	/*se aloca memorie pentru un element de tip Object*/
	obj = (Object)malloc(sizeof(struct obiect));
	strcpy(obj->nume, value);
	obj->nr = nr;
	
	/*daca se precizeaza cantitatea exacta,
	 *starea obiectului trebuie sa fie 0;
	 *altfel, starea ramane nemodificata
	 */
	if(simbol != '=')
		obj->stare = 1;
	else
		obj->stare = 0;
	
	/*se aloca memorie pentru un element de tip Room*/	
	r = (Room)malloc(sizeof(struct camera));
	strcpy(r->nume, key);
	r->obiecte = Hash_create(HMAX);
	
	/*se calculeaza indexul listei de obiecte din hashtable-ul de obiecte*/
	poz = MurmurHash(value);
	
	/*se insereaza obiectul in lista*/
	insertAt(&(r->obiecte->H[poz]), obj, 1); 
	  
	/*se insereaza camera in lista de camere din hashtable-ul de camere*/  
	insertAt(&h->H[MurmurHash(key)], r, 1);
	
	/*se creste numarul de intrari in hashtable-ul de camere*/
	h->n++;
	
	return 0;
}


void Hash_getObj(FILE *g, Hash h, unsigned int index, char *key, int *res1, int *res2)
{
	unsigned int i, j, k, n, r, s = 0;
	char **sol = NULL;
	Room p;
	Object q;
	
	n = sizeOfList(h->H[index]);
	
	/*se cauta in lista de camere, asociata camerei 'key'*/
	for(i = 0; i < n; i++)
	{
		/*se obtine cate un element de tip Room din nodurile listei*/
		p = (Room)elementAt(h->H[index], i);
		
		/*daca s-a gasit camera cautata*/
		if(strcmp(p->nume, key) == 0)
		{
			/*variabila res1 se actualizeaza(s-a gasit camera)*/
			*res1 = 1;
			
			/*se cauta obiectul in tot hashtable-ul de obiecte*/
			for(j = 0; j < p->obiecte->max; j++)
				if(isEmpty(p->obiecte->H[j]) == 1)
				{
					r = sizeOfList(p->obiecte->H[j]);
					
					/*se cauta obiectul in fiecare lista din hashtable*/
					for(k = 0; k < r; k++)
					{	
						/*se obtine un element de tip Object din nodurile fiecarei liste*/
						q = (Object)elementAt(p->obiecte->H[j], k);
						
						/*daca exista macar un obiect in camera*/
						if(q->nr > 0)
						{
							/*daca obiectul nu a fost introdus in vectorul de solutii*/	
							if(cauta(sol, s, q->nume) == 0)
							{
								sol = (char **)realloc(sol, ++s * sizeof(char *));
								sol[s-1] = (char *)malloc(MAX + 1);
								strcpy(sol[s-1], q->nume);
							}
							
							/*se actualizeaza variabila res2(s-au gasit obiecte)*/	
							*res2 = 1;
						}
					}
				}
				
			/*nu se mai cauta in lista; camera a fost gasita*/	
			break;
		}	
	}
	
	/*se scrie in fisier, vectorul de solutii*/
	if(s > 0)
		for(k = 0; k < s-1; k++)
			fprintf(g, "%s,", sol[k]);
					
	if(s > 0)
		fprintf(g, "%s\n", sol[s-1]);
	
	/*se elibereaza memoria*/				
	for(k = 0; k < s; k++)
		free(sol[k]);
	
	free(sol);
}


void Hash_getKeys(FILE *g, Hash h, int index, unsigned int *m, char *value, int *res1, int *res2)
{	
	unsigned int i, j, n, poz, r;
	Room p;
	Object q;
	
	/*se calculeaza indexul listei asociate obiectului 'value'*/
	poz = MurmurHash(value);
	
	n = sizeOfList(h->H[index]);
	
	/*se cauta in lista de pe pozitia 'index' din hashtable-ul de camere*/
	for(i = 0; i < n; i++)
	{
		/*se obtine un element de tip Room*/
		p = (Room)elementAt(h->H[index], i);
		
		/*se verifica daca exista o lista asociata obiectului cautat, 
		 *in hashtable-ul de obiecte al fiecarei camere
		 */
		if(p->obiecte->H[poz] != NULL)
		{
			r = sizeOfList(p->obiecte->H[poz]);
			
			/*se cauta obiectul in acea lista*/
			for(j = 0; j < r; j++)
			{
				q = (Object)elementAt(p->obiecte->H[poz], j);
				
				/*daca s-a gasit obiectul*/
				if(strcmp(q->nume, value) == 0)
				{
					/*se actualizeaza variabila res1;
					 *(s-a gasit obiectul)
					 */
					*res1 = 1;
					
					/*daca exista macar un obiect de acest tip*/
					if(q->nr > 0)
					{
						/*daca obiectul s-a gasit si in alte liste
						 *se afiseaza ',' pentru a continua afisarea
						 */
						if(*m > 0)
							fprintf(g, ",");	
						
						/*se actualizeaza variabila res2
						 *(exista macar un obiect)
						 */	
						*res2 = 1;
						
						/*se tine evidenta afisarilor*/
						(*m)++;
						
						fprintf(g, "%s", p->nume);
					}
					
					/*se opreste cautarea in lista de obiecte*/
					break;
				}
			}
		}	 	
	}
}


int main(int argc, char *argv[])
{
	Hash h;
	Room p;
	Object q;
	int nr, res1, res2, found;
	unsigned int n, m, i, j, k = 0, val, poz, index;
	char fisier[MAX], obiect[MAX], camera[MAX], simbol;
	
	FILE *f = fopen(argv[1], "r");
	
	if(f == NULL)
	{
		printf("Nu pot deschide fisierul '%s\n'", argv[1]);
	}
	
	/*se cauta primul punct din numele fisierului, cel pentru extensie*/
	for(i = strlen(argv[1]) - 1; i >= 0; i--)
		if(argv[1][i] == '.')
			break;
	
	/*se copiaza numele fisierului fara extensie*/		
	for(j = 0; j < i; j++)
		fisier[k++] = argv[1][j];
	fisier[k] = '\0';
	
	/*se pune extensia*/
	strcat(fisier, ".out");
	
	fscanf(f, "%u", &n);
	
	/*se face un hashtable de dimensiune HMAX*/
	h = Hash_create(HMAX);
	
	FILE *g = fopen(fisier, "w");
		
	while(!feof(f))
	{
		/*se citeste simbolul de la inceputul fiecarei linii*/
		fscanf(f, "%c", &simbol);
		
		/*daca trebuie updatate sau inserate perechi camera-obiect*/
		if(simbol == '=' || simbol == '+' || simbol == '-') 
		{ 	
			fscanf(f, "%d %s %s", &nr, obiect, camera);
			
			if(simbol == '-')
				nr = -nr;
			
			/*se insereaza/updateaza perechea*/
			Hash_insert(h, camera, obiect, nr, simbol);
		}
		
		
		if(simbol == '?')		
		{	
			fscanf(f, "%s", camera); 
		
			res1 = 0;
			res2 = 0;
			
			/*se calculeaza indexul listei asociate camerei in hashtable-ul de camere*/
			poz = MurmurHash(camera);
			
			/*daca lista e goala*/
			if(h->H[poz] == NULL)
				fprintf(g, "nu se stie\n");
			else
				{
					/*se obtin obiectele din lista asociata camerei cautate*/
					Hash_getObj(g, h, poz, camera, &res1, &res2);
					
					/*daca nu s-a gasit camera in acea lista*/
					if(res1 == 0)
						fprintf(g, "nu se stie\n");
					else
				
						/*daca nu a existat macar un obiect in acea camera*/
						if(res2 == 0)
							fprintf(g, "nimic\n");
				}			
		}	
			
		if(simbol == '@') 
		{		
			fscanf(f, "%s", obiect);
			
			m = 0;
			found = 0;
			val = 0;
			
			/*se cauta in fiecare lista din hashtable-ul de camere*/
			for(i = 0; i < h->max; i++)
			{
				res1 = 0;
				res2 = 0;
			
				if(h->H[i] != NULL)
				{
					/*se afiseaza camerele din fiecare lista, care contin obiectul cautat*/
					Hash_getKeys(g, h, i, &m, obiect, &res1, &res2);
					
					/*daca macar o camera a avut cel putin un obiect de tipul celui cautat*/
					if(res2 == 1)	
						val = 1;
					else
						/*daca s-a gasit camera, dar nu s-au gasit obiecte*/
						if(res1 == 1)	
							found = 1;
				}
			}	

			/*se incheie afisarea(cel putin o camera a fost afisata)*/
			if(val == 1)
				fprintf(g, "\n");
			else
				{
					/*daca macar s-a gasit camera, dar nu si obiecte*/
					if(found == 1)
						fprintf(g, "nicaieri\n");
					else
						fprintf(g, "nu se stie\n");
				}
		}
		
		if(simbol == '#') 
		{
			fscanf(f, "%s %s", obiect, camera);
			
			found = 0; 
			
			/*se calculeaza indexul listei asociate camerei in hashtable-ul de camere*/
			index = MurmurHash(camera);
			
			/*daca lista e goala*/
			if(h->H[index] == NULL)
				fprintf(g, "nu se stie\n");
			else
				{
					n = sizeOfList(h->H[index]);
					
					/*se cauta in lista*/
					for(i = 0; i < n; i++)
					{
						/*se obtine cate un element de tip Room*/
						p = (Room)elementAt(h->H[index], i);
						
						/*daca s-a gasit camera*/
						if(strcmp(p->nume, camera) == 0)
						{
							/*se calculeaza indexul listei asociate obiectului cautat,
							 *in hashtable-ul de obiecte al camerei cautate
							 */
							poz = MurmurHash(obiect);
							
							if(isEmpty(p->obiecte->H[poz]) == 1)
							{
								m = sizeOfList(p->obiecte->H[poz]);
								
								/*se cauta in acea lista, fiindca nu e vida*/
								for(j = 0; j < m; j++)
								{	
									q = (Object)elementAt(p->obiecte->H[poz], j);
									
									/*daca s-a gasit si obiectul*/
									if(strcmp(q->nume, obiect) == 0)
									{	
										found = 1;
										
										/*daca nu se cunoaste continutul camerei*/
										if(q->stare == 1)
											fprintf(g, "nu se stie\n");
										else
											/*se afiseaza cate obiecte de tipul cautat
											 *exista in camera
											 */
											fprintf(g, "%u\n", q->nr);
										
										/*se opreste cautarea*/	
										break;	
									}
								}
							}	
						}
					}
					
					/*daca perechea obiect-camera nu a fost gasita*/
					if(found == 0)
						fprintf(g, "nu se stie\n");
				}
		}		
	}	  
	
	Hash_free(&h);

	fclose(g);
	fclose(f);
		
	return 0;
}
