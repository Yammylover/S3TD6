#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct struct_tree{
	struct struct_tree* ls;
	struct struct_tree* rs;
	int value;
	int eq;
} Tree;

Tree* createAVL(int a){
	Tree* Na=malloc(sizeof(Tree));
	if(Na==NULL){
		printf("Erreur mémoire\n");
		exit(1);
	}
	Na->value=a;
	Na->eq=0;
	Na->ls=NULL;
	Na->rs=NULL;
	return Na;
}

int min(int a, int b, int c, int d){
	int min=a;
	if(min>b){
		min=b;
	}
	if(min>c){
		min=c;
	}
	if(min>d){
		min=d;
	}
	return min;
}

int max(int a, int b, int c, int d){
	int max=a;
	if(max<b){
		max=b;
	}
	if(max<c){
		max=c;
	}
	if(max<d){
		max=d;
	}
	return max;
}

Tree* insertAVL(Tree* avl, int a, int* h){
	if(avl==NULL){	//le noeud est créé, on met l'équilibre de son père à 1 pour être modifié plus tard;
		*h=1;
		return createAVL(a);
	}
	if (a<avl->value){	//le noeud est créé à gauche, donc l'équilibre va dans le sens gauche
		avl->ls=insertAVL(avl->ls,a,h);
		*h=-*h;
	}
	if(a>avl->value){	//le noeud est créé à droite, donc l'équilibre reste dans le sens droit
		avl->rs=insertAVL(avl->rs,a,h);
	}
	*h=0;	//le noeud n'est pas créé
	if(*h!=0){	//l'équilibre est changé
		avl->eq+=*h;	//ajout de h à l'équilibre
		//equilibrage
		if(avl->eq==0){	//equilibre a été nullifié
			*h=0;	//
		} else {
			*h=1;
		}
	}
	return avl;
}

Tree* ajouterfilsgauche(Tree* avl, int a){
	if(avl==NULL){
		printf("arbre nul\n");
		exit(6);
	}
	avl->eq=avl->eq-1;
	if(avl->ls!=NULL){
		ajouterfilsgauche(avl->ls,a);
	} else {
		avl->ls=createAVL(a);
	}
	return avl->ls;
}

Tree* ajouterfilsdroit(Tree* avl, int a){
	if(avl==NULL){
		printf("arbre nul\n");
		exit(7);
	}
	avl->eq=avl->eq+1;
	if(avl->rs!=NULL){
		ajouterfilsdroit(avl->rs,a);
	} else {
		avl->rs=createAVL(a);
	}
	return avl->rs;
}

Tree* rotationgauche(Tree* arbre){
	//sécurité
	if(arbre==NULL){
		printf("Erreur 67\n");
		exit(67);
	}
	//variables
	Tree* pivot;
	int eq_p, eq_a;
	//process
	pivot=arbre->rs;
	arbre->rs=pivot->ls;
	pivot->ls=arbre;
	eq_a=arbre->eq;
	eq_p=pivot->eq;
	arbre->eq=eq_a-max(eq_p,0,-100,-100)-1;
	pivot->eq=min(eq_a-2,eq_a+eq_p-2,eq_p-1,100);
	arbre=pivot;
	return arbre;
}

Tree* rotationdroite(Tree* arbre){
	//sécurité
	if(arbre==NULL){
		printf("Erreur 68\n");
		exit(68);
	}
	//variables
	Tree* pivot;
	int eq_p, eq_a;
	//process
	pivot=arbre->ls;
	arbre->ls=pivot->rs;
	pivot->rs=arbre;
	eq_a=arbre->eq;
	eq_p=pivot->eq;
	arbre->eq=eq_a-min(eq_p,0,100,100)+1;
	pivot->eq=max(eq_a+2,eq_a+eq_p+2,eq_p+1,-100);
	arbre=pivot;
	return arbre;
}

Tree* doublerotationdroite(Tree* a){
	a->ls=rotationgauche(a->ls);
	return rotationdroite(a);
}

Tree* doublerotationgauche(Tree* a){
	a->rs=rotationdroite(a->rs);
	return rotationgauche(a);
}

int postfixe(Tree* avl){
	if(avl->ls!=NULL){
		postfixe(avl->ls);
	}
	if(avl->rs!=NULL){
		postfixe(avl->rs);
	}
	printf("%d(%d)",avl->value,avl->eq);
}

int prefixe(Tree* avl){
	printf("%d(%d)",avl->value,avl->eq);
	if(avl->ls!=NULL){
		prefixe(avl->ls);
	}
	if(avl->rs!=NULL){
		prefixe(avl->rs);
	}
}

Tree* equilibreAVL(Tree* a){
	if(a==NULL){
		printf("Erreur 78\n");
		exit(68);
	}
	if(a->eq>=2){
		if(a->rs->eq<=0){
			a->rs=rotationdroite(a->rs);
		}
		return rotationgauche(a);
	} else if(a->eq<=-2){
		if(a->ls->eq>=0){
			a->ls=rotationgauche(a->ls);
		}
		return rotationdroite(a);
	}
}

int main(){
	int a,h;
	Tree* avl=NULL;
	avl=createAVL(10);
	insertAVL(avl,5,&h);
	insertAVL(avl,20,&h);
	insertAVL(avl,15,&h);
	insertAVL(avl,13,&h);
	insertAVL(avl,17,&h);
	insertAVL(avl,26,&h);
	avl->eq=2;
	avl->rs->eq=-1;
	prefixe(avl);
	printf("\n");
	avl=equilibreAVL(avl);
	prefixe(avl);
	printf("\n");
	return 0;
}
