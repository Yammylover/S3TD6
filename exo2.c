#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct struct_tree{
	struct struct_tree* ls;
	struct struct_tree* rs;
	int value;
	int eq;
} Tree;

void checkpoint(int* c){//fonction de débogage
	printf("Checkpoint %d\n",*c);
	fflush(stdout);
	*c=*c+1;
}

void anyradiance(int a, char* string){
	printf("Erreur %d à %s\n",a,string);
	exit(a);	
}

Tree* createAVL(int a){
	Tree* Na=malloc(sizeof(Tree));
	if(Na==NULL){
		anyradiance(1,"mémoire");
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
		exit(78);
	}
	if(a->eq>=2){
		if(a->rs->eq<0){
			a->rs=rotationdroite(a->rs);
		}
		return rotationgauche(a); //a=rotationgauche(a)
	} else if(a->eq<=-2){
		if(a->ls->eq>0){
			a->ls=rotationgauche(a->ls);
		}
		return rotationdroite(a);
	}
	return a;//IMPORTANT!!!!!!!!!!!!
}

Tree* insertAVL(Tree* avl, int a, int* h){
	if(h==NULL){//vérification variables
		printf("Erreur 56\n");
		exit(56);
	}
	if(avl==NULL){	//le noeud est créé, on met l'équilibre de son père à 1 pour être modifié plus tard;
		*h=1;
		return createAVL(a);
	} else if (a<avl->value){	//le noeud est créé à gauche, donc l'équilibre va dans le sens gauche
		avl->ls=insertAVL(avl->ls,a,h);
		*h=-*h;
	}else if(a>avl->value){	//le noeud est créé à droite, donc l'équilibre reste dans le sens droit
		avl->rs=insertAVL(avl->rs,a,h);
	}else {
		*h=0;	//le noeud n'est pas créé
	}
	if(*h!=0){	//l'équilibre est changé
		avl->eq+=*h;	//ajout de h à l'équilibre
		avl=equilibreAVL(avl);
		if(avl->eq==0){	//equilibre a été nullifié
			*h=0;	//
		} else {
			*h=1;
		}
	}
	return avl;
}

Tree* insertAVL2(Tree* avl, int a, int* h){
	if(avl==NULL){	//on créé la branche
		*h=1;
		return createAVL(a);
	}else if(a<avl->value){//on crée le noeud à gauche
		avl->ls=insertAVL2(avl->ls,a,h);	//on rééxécute la fonction au fils gauche
		*h=-*h;	//on inverse la valeur de h, donc si le fils est créé, c'est -1, sinon c'est 0
	}else if(a>avl->value){// on créé le noeud a droite
		avl->rs=insertAVL2(avl->rs,a,h);
		//pas de modifs de h, car si c'est >0, ça reste identique, sinon, c'est que le noeud n'a pas été rajouté
	} else  {
		*h=0;
	}
	if(*h!=0){
		avl->eq+=*h;
		avl=equilibreAVL(avl);
		if(avl->eq==0){//l'équilibre a été nullifié, on met le poids du noeud supérieur à 0
			*h=0;
		} else {//l'équilibre n'est pas nullifié, on garde le poids pour le noeud supérieur
			*h=1;
		}
	}
	return avl;//retourner le noeud actuel si il n'y a pas eu création de noeud
}

Tree* suppminAVL(Tree* a, int* h, int* e){
	Tree* tmp;
	if(a->ls==NULL){
		*e=a->value;
		tmp=a;
		a=a->rs;
		free(tmp);
		*h=-1;
		return a;
	} else{
		a->ls=suppminAVL(a->ls,h,e);
		*h=-*h;
	}
	if(*h!=0){
		a->eq=a->eq+*h;
		a=equilibreAVL(a);
		if(a->eq==0){
			*h=-1;
		} else {
			*h=0;
		}
	}
	return a;
}

Tree* supAVL(Tree* avl, int a,int* h){
	Tree* tmp;
	if(avl==NULL){
		*h=0;
		return avl;
	} else if(a<avl->value){
		avl->ls=supAVL(avl,a,h);
		*h=-*h;
	} else if(a>avl->value){
		avl->rs=supAVL(avl,a,h);
	} else if(avl->rs!=NULL){
		avl->rs=suppminAVL(avl,h,&(avl->value));
	} else {
		tmp=avl;
		avl=avl->rs;
		free(tmp);
		*h=-1;
	}
	if(*h!=0){
		avl->eq=avl->eq+*h;
		if(avl->eq==0){
			*h=-1;
		} else {
			*h=0;
		}
	}
	return avl;
}

int main(){
	int a,h,checker=1;
	Tree* avl=NULL;
	avl=createAVL(10);
	avl=insertAVL(avl,5,&h);	//avl=insert parce que l'équilibrage change le premier membre de l'arbre
	avl=insertAVL(avl,20,&h);
	avl=insertAVL(avl,15,&h);
	avl=insertAVL(avl,13,&h);
	avl=insertAVL(avl,17,&h);
	avl=insertAVL(avl,26,&h);
	//checkpoint(&checker);
	prefixe(avl);
	printf("\n");
	avl=supAVL(avl,20,&h);
	prefixe(avl);
	printf("\n");
	return 0;
}
