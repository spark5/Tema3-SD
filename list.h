#ifndef _LIST_H
#define _LIST_H

typedef struct l_nod {
	void *value;
	struct l_nod *next;
	struct l_nod *prev;
}L_Nod;
 
typedef L_Nod* List;
 
 
/* adaugare intr-o pozitie data (idem, pentru valoarea intoarsa)*/
void insertAt(List *l, void* elem, long poz);
 
/* stergerea elementului dintr-o pozitie data */
void* delAt(List *l, long poz);
 
/* intoarce elementul dintr-o pozitie data (NULL daca pozitia nu e in lista)*/
void* elementAt(List l, long poz);
 
/* intoarce lungimea listei*/
long sizeOfList(List l);
 
/* test lista vida, 1 daca e vida, 0 altfel*/
long isEmpty(List l);

#endif
