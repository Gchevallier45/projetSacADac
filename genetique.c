#include "genetique.h"

void initPopulation(Population *population, int taillePopulation, int typeSolutions){
    population->taillePopu = taillePopulation;
    population->type = typeSolutions;
    population->solutions = (int*)malloc(taillePopulation*sizeof(int*));
}

void creerPopulation(Population *population, int taillePopulation, int typeSolutions, Instance *instance){
    population->taillePopu = taillePopulation;
    population->type = typeSolutions;
    population->solutions = (int*)malloc(taillePopulation*sizeof(int*));
    if(typeSolutions == DIRECT){
        for(int i=0;i<taillePopulation;i++){
            int *solution = malloc(instance->objetNb*sizeof(int));
            Direct(solution,instance,1);
            population->solutions[i] = solution;
        }
    }
    else{
        for(int i=0;i<taillePopulation;i++){
            int *solution = malloc(instance->objetNb*sizeof(int));
            decRatioValPoidsPick(solution, instance);
            population->solutions[i] = solution;
        }
    }
}

void detruirePopulation(Population *population){
    for(int i=0;i<population->taillePopu;i++){
        free(population->solutions[i]);
    }
    free(population->solutions);
    free(population);
}

/** Metaheuristique génétique direct
 * @param tab le tableau dans lequel sera stocké la solution
 * @param instance l'instance à utiliser pour générer la solution
 * @param NbIteMax le nombre d'itérations maximum sans trouver une meilleure solution courante
 * Préconditions : tab non nul, et d'une taille correspondant au nombre d'objets de l'instance
 */
void metaGenetiqueDirecte(int* tab, Instance *instance, int nbIteMax, int taillePopu, int pMut){
    Population *pop = malloc(sizeof(Population));
    creerPopulation(pop,taillePopu,DIRECT,instance);
    int *solutionBest = malloc(instance->objetNb*sizeof(int)); //En codage direct
    int fbest = 0;
    int indiceSolutionBest  = 0;
    for(int i=0;i<pop->taillePopu;i++){
        int val = directResultat(pop->solutions[i],instance);
        if(val > fbest){
            fbest = val;
            indiceSolutionBest = i;
        }
    }

    memcpy(solutionBest,pop->solutions[indiceSolutionBest],instance->objetNb*sizeof(int)); //Copie de solutionCourante dans solutionBest

    int nbIte = 0;
    while(nbIte < nbIteMax){
        Population *enfant = malloc(sizeof(Population));
        initPopulation(enfant,taillePopu,DIRECT);

        for(int i=0;i<taillePopu/2;i++){
            int parentsSelect[4];
            int parents[2];

            //On séléctionne des parents aléatoires
            parents[0] = rand() % taillePopu;
            parents[1] = rand() % taillePopu;
            while(parents[0] == parents[1]){
                parents[1] = rand() % taillePopu;
            }

            //On créée des enfants
            int pointCroisement = rand() % instance->objetNb;
            int *enfant1 = malloc(instance->objetNb*sizeof(int));
            int *enfant2 = malloc(instance->objetNb*sizeof(int));
            procreerDirect(pop->solutions[parents[0]],pop->solutions[parents[1]],enfant1,enfant2,instance);

            enfant->solutions[i*2] = enfant1;
            enfant->solutions[i*2+1] = enfant2;
            int valEnfant1,valEnfant2;
            valEnfant1 = directResultat(enfant1,instance);
            valEnfant2 = directResultat(enfant2,instance);
            printf("-----------\n");
            printf("val enfant 1 %d, val enfant 2 %d\n",valEnfant1,valEnfant2);
            //affSoluce(pop->solutions[parents[0]],instance->objetNb);
            //affSoluce(pop->solutions[parents[1]],instance->objetNb);
            //affSoluce(enfant1,instance->objetNb);
            //printf("point crois %d, deux copie %d, total %d\n\n",pointCroisement,instance->objetNb-pointCroisement,instance->objetNb-pointCroisement+pointCroisement);

            //printf("PARENT 1 : %d, PARENT 2 : %d\n",parents[0],parents[1]);
        }

        for(int i=0;i<taillePopu;i++){
            int resultat = directResultat(enfant->solutions[i],instance);
            if(resultat > fbest){
                fbest = resultat;
                memcpy(solutionBest,enfant->solutions[i],instance->objetNb*sizeof(int));
            }
            int tirageAleatoireMutation = rand() % 100 + 1; //On tire un nombre entre 1 et 100
            if(tirageAleatoireMutation < pMut){ //si le nombre tiré est inférieur à la proba de mutation, on mute l'enfant
                mutationDirect(enfant->solutions[i],instance);
                printf("mutationnnnnnnnnn\n");
                if(resultat > fbest){
                    fbest = resultat;
                    memcpy(solutionBest,enfant->solutions[i],instance->objetNb*sizeof(int));
                }
            }
        }

        nbIte++;
    }

    memcpy(tab,solutionBest,instance->objetNb*sizeof(int)); //Copie de solutionCourante dans solutionBest

    detruirePopulation(pop);
    free(solutionBest);
}

void procreerDirect(int *parent1, int *parent2, int *enfant1, int *enfant2, Instance *instance){
    int pointCroisement = rand() % instance->objetNb;
    memcpy(enfant1,parent1,pointCroisement*sizeof(int));
    memcpy(enfant1+pointCroisement,parent2+pointCroisement,(instance->objetNb-pointCroisement)*sizeof(int));
    memcpy(enfant2,parent2,pointCroisement*sizeof(int));
    memcpy(enfant2+pointCroisement,parent1+pointCroisement,(instance->objetNb-pointCroisement)*sizeof(int));

    int valEnfant1;
    int valEnfant2;
    while(directFaisable(enfant1,instance) == 0){
        enfant1[rand() % instance->objetNb] = 0;
    }
    while(directFaisable(enfant2,instance) == 0){
        enfant2[rand() % instance->objetNb] = 0;
    }
}

void mutationDirect(int *membre, Instance *instance){
    //On mute l'enfant
    int randNb;
    do{
        randNb = rand() % instance->objetNb;
    }while(membre[randNb] == 0);

    //On corrige la mutation si elle n'est pas faisable
    while(directFaisable(membre,instance) == 0){
        membre[rand() % instance->objetNb] = 0;
    }
}
