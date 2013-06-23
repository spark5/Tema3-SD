#include <stdio.h>
#include <stdlib.h>
#include "list.h"


/* adaugare intr-o pozitie data (idem, pentru valoarea intoarsa)*/
void insertAt(List *l, void* elem, long poz)
{
	if(*l == NULL)
	{
		List p;
		p = (List)malloc(sizeof(L_Nod));
		p->value = elem;
		p->next = p;
		p->prev = p;
		*l = p;
	}
		
	else 
		{
			long i;
			
			L_Nod *e;
			List p = *l;
			e = (List)malloc(sizeof(L_Nod));
			e->value = elem;
			
			for(i = 1; i < poz; i++)			
				p = p->next;
			e->next = p->next;
			e->next->prev = e;
			e->prev = p;
			p->next = e;	
			
		
		}
}


/* stergerea elementului dintr-o pozitie data */
void* delAt(List *l, long poz)
{
	List p = *l;
	long i;
		
	for(i = 0; i < poz; i++)
		p = p->next;

			
	p->prev->next = p->next;
	p->next->prev = p->prev;
	
	return p->value;

}


/* intoarce elementul dintr-o pozitie data (NULL daca pozitia nu e in lista)*/
void* elementAt(List l, long poz)
{
	List p = l;
	long i;
	
	for(i = 0; i < poz; i++)
		p = p->next;

	return p->value;
}


/* intoarce lungimea listei*/
long sizeOfList(List l)
{
	List p=l;
	long c = 0;
	
	do{
	    p = p->next;
	    c++;	
	}while(p != l);

	return c;
}		


/* test lista vida, 1 daca e vida, 0 altfel*/
long isEmpty(List l)
{
	if(l == NULL)
		return 0;
	
	return 1;
}

