#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <ctype.h>
#include "../include/InputText.h"
#include "../include/ExtraFunctions.h"
#include "../include/CommonIncludes.h"
#include "../include/Tree.h"
#include "../include/Differentiator.h"

int const_calculation(Node* N, size_t* changes)
{
    int nan_found = 0;

    if (N == NULL)  return OBJECT_N_FOUND;

    #ifdef DEBUG_ON
    printf("const calc in node ");
    fprint_arg(stdout, N);
    printf("\n");
    #endif

    if (ND == var) return OBJECT_FOUND;
    if ((L == NULL) && (R == NULL)) return OBJECT_N_FOUND;

    nan_found |= const_calculation(L, changes);
    nan_found |= const_calculation(R, changes);

    if (nan_found & OBJECT_FOUND) return OBJECT_FOUND;

    if (ND == operation)
    {
        switch (NVO)
        {
        case add_op:
            NVN = NLVN + NRVN;
            break;
        case sub_op:
            NVN = NLVN - NRVN;
            break; 
        case mul_op:
            NVN = NLVN * NRVN;
            break;       
        case divis_op:
            NVN = NLVN / NRVN;
            break;        
        case pow_op:
            NVN = pow(NLVN, NRVN);
            break;
        case assign_op:
            break;
        case null_op:
        default:
            printf("Unknown operation\n");
            break;
        }
        ND = number;

        node_dtor_all(L);
        node_dtor_all(R);

        L =  NULL;
        R = NULL;

        (*changes)++;
    }
    else if (ND == func)
    {   
        double val = 0;
        double arg = NRVN;

        switch (NVF)
        {
        case sin_f:
            val = sin(arg);
            break;
        case cos_f:
            val = cos(arg);
            break; 
        case tg_f:
            val = tan(arg);
            break;  
        case ctg_f:
            val = 1/(tan(arg));
            break;
        case sh_f:
            val = sinh(arg);
            break;
        case ch_f:
            val = cosh(arg);
            break;
        case th_f:
            val = tanh(arg);
            break;
        case cth_f:
            val = 1/(tanh(arg));
            break;
        case ln_f:
            val = log(arg);
            break;
        case exp_f:
            val = exp(arg);
            break;
        case null_f:
        default:
            printf("Wrong func!\n");
            break;
        }
        ND = number;
        free(NV);
        NV = val_double(val);

        node_dtor_all(L);
        node_dtor_all(R);

        L =  NULL;
        R = NULL;

        (*changes)++;
    }
    return OBJECT_N_FOUND;
}


void action_with_zero(Node* N, size_t* changes)
{
    if (N == NULL) return;

    action_with_zero(L, changes);
    action_with_zero(R, changes);

    if ((ND == operation) && (NVO == mul_op))
    {
        if ((LD == number) && (CmpDbl(NLVN, (double)0)))
        {
            node_dtor_all(R);
            node_dtor_all(L);

            (*changes)++;

            ND = number;
            NVN = (double)0;

            L = NULL;
            R = NULL;

            return;
        }
        if ((RD == number) && (CmpDbl(NRVN, (double)0)))
        {
            node_dtor_all(L);
            node_dtor_all(R);

            (*changes)++;

            ND = number;
            NVN = (double)0;

            L = NULL;
            R = NULL;

            return;
        }
    }
    else if ((ND == operation) && (NVO == pow_op))
    {
        if ((LD == number) && (CmpDbl(NLVN, (double)0)))
        {
            node_dtor_all(R);
            node_dtor_all(L);

            (*changes)++;

            ND = number;
            NVN = (double)0;

            L = NULL;
            R = NULL;

            return;
        }
        if ((RD == number) && (CmpDbl(NRVN, (double)0)))
        {
            node_dtor_all(L);
            node_dtor_all(R);

            (*changes)++;

            ND = number;
            NVN = (double)1;

            L = NULL;
            R = NULL;

            return;
        }
    }
    else if ((ND == operation) && (NVO == sub_op))
    {
        if ((LD == number) && (CmpDbl(NLVN, (double)0)))
        {
            NRVN = (-1) * (NRVN);
            node_dtor_one(N, right);
            (*changes)++;

            return;
        }
        if ((RD == number) && (CmpDbl(NRVN, (double)0)))
        {
            node_dtor_one(N, left);
            (*changes)++;

            return;
        }
    }
    else if ((ND == operation) && (NVO == add_op))
    {
        if ((LD == number) && (CmpDbl(NLVN, (double)0)))
        {
            node_dtor_one(N, right);
            (*changes)++;

            return;
        }
        if ((RD == number) && (CmpDbl(NRVN, (double)0)))
        {
            node_dtor_one(N, left);
            (*changes)++;

            return;
        }
    }

    return;
}

void action_with_one(Node* N, size_t* changes)
{
    if (N == NULL) return;

    action_with_one(L, changes);
    action_with_one(R, changes);

    if ((ND == operation) && (NVO == mul_op))
    {
        if ((LD == number) && (CmpDbl(NLVN, (double)1)))
        {
            node_dtor_one(N, right);
            (*changes)++;

            return;
        }
        if ((RD == number) && (CmpDbl(NRVN, (double)1)))
        {
            node_dtor_one(N, left);
            (*changes)++;

            return;
        }
    }
    else if ((ND == operation) && (NVO == pow_op))
    {
        if ((LD == number) && (CmpDbl(NLVN, (double)1)))
        {
            node_dtor_all(R);
            node_dtor_all(L);

            (*changes)++;

            ND = number;
            NVN = (double)1;

            L = NULL;
            R = NULL;

            return;
        }
        if ((RD == number) && (CmpDbl(NRVN, (double)1)))
        {
            node_dtor_one(N, left);
            (*changes)++;

            return;
        }
    }
    return;
}

int nan_search(Node* N)
{
    if (N == NULL) return OBJECT_N_FOUND;

    #ifdef DEBUG_ON
    printf("Curr node is ");
    fprint_arg(stdout, N);
    printf("\n");
    #endif

    if (ND == var)
    {
        return OBJECT_FOUND;
    }

    int nan_found = nan_search(L) | nan_search(R);

    #ifdef DEBUG_ON
    printf("For node ");
    fprint_arg(stdout, N);
    printf(" nan_found = %d\n", nan_found);
    #endif

    if (nan_found & OBJECT_FOUND)
    {
        return OBJECT_FOUND;
    }
    return OBJECT_N_FOUND;
}

void tree_optimize(Node* N)
{
    size_t changes = 1;

    while (changes != 0)
    {
        changes = 0;
        const_calculation(N, &changes);
        action_with_zero(N, &changes);
        action_with_one(N, &changes);

        #ifdef DEBUG_ON
        printf("After opt cycle\n");
        node_print(N, stdout);
        printf("\n");
        #endif
    }
}

void var_substitution(Node* N, double var_val)
{
    if (!N) return;

    if (!L & !R)
    {
        if (ND == var)
        {
            ND = number;
            free(NVV);
            free(NV);
            NV = val_double(var_val);
            return;   
        }
   }

   var_substitution(L, var_val);
   var_substitution(R, var_val);
   return; 
}

double tree_calc(Node* N, double var_val)
{
    Node* calc_tree = copy_node(N);
    var_substitution(calc_tree, var_val);

    #ifdef DEBUG_ON
    printf("\nSubstitution\n");        
    node_print(calc_tree, stdout);      
    printf("\nOptimisation\n");         
    #endif
    
    tree_optimize(calc_tree);

    #ifdef DEBUG_ON
    node_print(calc_tree, stdout);
    printf("\n");
    #endif

    double val = calc_tree->val->number;
    node_dtor_all(calc_tree);

    return val;
}
