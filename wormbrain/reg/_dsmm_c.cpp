#define NPY_NO_DEPRECATED_API NPY_1_9_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>
#include "dsmm.hpp" //This is inside dsmm/ in the root of the repository

// Define functions below

static PyObject *_dsmmc_bare(PyObject *self, PyObject *args);

/////// Python-module-related functions and tables

// The module's method table
static PyMethodDef _dsmm_cMethods[] = {
    {"_dsmmc_bare", _dsmmc_bare, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}
};

// The module definition function
static struct PyModuleDef _dsmm_c = {
    PyModuleDef_HEAD_INIT,
    "_dsmm_c",
    NULL, // Module documentation
    -1,
    _dsmm_cMethods
};

// The module initialization function
PyMODINIT_FUNC PyInit__dsmm_c(void) { 
        import_array(); //Numpy
        return PyModule_Create(&_dsmm_c);
};

static PyObject *_dsmmc_bare(PyObject *self, PyObject *args) {

    int M, N, D;
    double beta, lambda, neighbor_cutoff, alpha, gamma0, conv_epsilon, eq_tol;
    //bool releaseGIL;
    PyObject *X_o, *Y_o, *pwise_dist_o, *pwise_distYY_o, *Gamma_o, *CDE_term_o;
    PyObject *w_o, *F_t_o, *wF_t_o, *wF_t_sum_o, *p_o, *u_o, *Match_o;
    PyObject *hatP_o, *hatPI_diag_o, *hatPIG_o, *hatPX_o, *hatPIY_o;
    PyObject *G_o, *W_o, *GW_o, *sumPoverN_o, *expAlphaSumPoverN_o;
    
    if(!PyArg_ParseTuple(args, "OOiiidddddddOOOOOOOOOOOOOOOOOOOOO", 
        &X_o, &Y_o, &M, &N, &D, 
        &beta, &lambda, &neighbor_cutoff,
        &alpha, &gamma0,
        &conv_epsilon, &eq_tol,
        &pwise_dist_o, &pwise_distYY_o,
        &Gamma_o, &CDE_term_o,
        &w_o, &F_t_o, &wF_t_o, &wF_t_sum_o,
        &p_o, &u_o, &Match_o,
        &hatP_o, &hatPI_diag_o, &hatPIG_o, &hatPX_o, &hatPIY_o,
        &G_o, &W_o, &GW_o,
        &sumPoverN_o, &expAlphaSumPoverN_o)) return NULL;
    
    PyArrayObject *X_a = (PyArrayObject*) PyArray_FROM_OT(X_o, NPY_FLOAT64);
    PyArrayObject *Y_a = (PyArrayObject*) PyArray_FROM_OT(Y_o, NPY_FLOAT64);
    PyArrayObject *pwise_dist_a = (PyArrayObject*) PyArray_FROM_OT(pwise_dist_o, NPY_FLOAT64);
    PyArrayObject *pwise_distYY_a = (PyArrayObject*) PyArray_FROM_OT(pwise_distYY_o, NPY_FLOAT64);
    PyArrayObject *Gamma_a = (PyArrayObject*) PyArray_FROM_OT(Gamma_o, NPY_FLOAT64);
    PyArrayObject *CDE_term_a = (PyArrayObject*) PyArray_FROM_OT(CDE_term_o, NPY_FLOAT64);
    PyArrayObject *w_a = (PyArrayObject*) PyArray_FROM_OT(w_o, NPY_FLOAT64);
    PyArrayObject *F_t_a = (PyArrayObject*) PyArray_FROM_OT(F_t_o, NPY_FLOAT64);
    PyArrayObject *wF_t_a = (PyArrayObject*) PyArray_FROM_OT(wF_t_o, NPY_FLOAT64);
    PyArrayObject *wF_t_sum_a = (PyArrayObject*) PyArray_FROM_OT(wF_t_sum_o, NPY_FLOAT64);
    PyArrayObject *p_a = (PyArrayObject*) PyArray_FROM_OT(p_o, NPY_FLOAT64);
    PyArrayObject *u_a = (PyArrayObject*) PyArray_FROM_OT(u_o, NPY_FLOAT64);
    PyArrayObject *Match_a = (PyArrayObject*) PyArray_FROM_OT(Match_o, NPY_INT32);
    PyArrayObject *hatP_a = (PyArrayObject*) PyArray_FROM_OT(hatP_o, NPY_FLOAT64);
    PyArrayObject *hatPI_diag_a = (PyArrayObject*) PyArray_FROM_OT(hatPI_diag_o, NPY_FLOAT64);
    PyArrayObject *hatPIG_a = (PyArrayObject*) PyArray_FROM_OT(hatPIG_o, NPY_FLOAT64);
    PyArrayObject *hatPX_a = (PyArrayObject*) PyArray_FROM_OT(hatPX_o, NPY_FLOAT64);
    PyArrayObject *hatPIY_a = (PyArrayObject*) PyArray_FROM_OT(hatPIY_o, NPY_FLOAT64);
    PyArrayObject *G_a = (PyArrayObject*) PyArray_FROM_OT(G_o, NPY_FLOAT64);
    PyArrayObject *W_a = (PyArrayObject*) PyArray_FROM_OT(W_o, NPY_FLOAT64);
    PyArrayObject *GW_a = (PyArrayObject*) PyArray_FROM_OT(GW_o, NPY_FLOAT64);
    PyArrayObject *sumPoverN_a = (PyArrayObject*) PyArray_FROM_OT(sumPoverN_o, NPY_FLOAT64);
    PyArrayObject *expAlphaSumPoverN_a = (PyArrayObject*) PyArray_FROM_OT(expAlphaSumPoverN_o, NPY_FLOAT64);
        
    // Check that the above conversion worked, otherwise decrease the reference
    // count and return NULL.                                 
    if (X_a == NULL || Y_a == NULL || pwise_dist_a == NULL || pwise_distYY_a == NULL
        || Gamma_a == NULL || CDE_term_a == NULL || w_a == NULL || F_t_a == NULL
        || wF_t_a == NULL || wF_t_sum_a == NULL || p_a == NULL || u_a == NULL
        || Match_a == NULL
        || hatP_a == NULL || hatPI_diag_a == NULL || hatPIG_a == NULL || hatPX_a == NULL
        || hatPIY_a == NULL || G_a == NULL || W_a == NULL || GW_a == NULL
        || sumPoverN_a == NULL || expAlphaSumPoverN_a == NULL
        ) {
        Py_XDECREF(X_a);
        Py_XDECREF(Y_a);
        Py_XDECREF(pwise_dist_a);
        Py_XDECREF(pwise_distYY_a);
        Py_XDECREF(Gamma_a);
        Py_XDECREF(CDE_term_a);
        Py_XDECREF(w_a);
        Py_XDECREF(F_t_a);
        Py_XDECREF(wF_t_a);
        Py_XDECREF(wF_t_sum_a);
        Py_XDECREF(p_a);
        Py_XDECREF(u_a);
        Py_XDECREF(Match_a);
        Py_XDECREF(hatP_a);
        Py_XDECREF(hatPI_diag_a);
        Py_XDECREF(hatPIG_a);
        Py_XDECREF(hatPX_a);
        Py_XDECREF(hatPIY_a);
        Py_XDECREF(G_a);
        Py_XDECREF(W_a);
        Py_XDECREF(GW_a);
        Py_XDECREF(sumPoverN_a);
        Py_XDECREF(expAlphaSumPoverN_a);
        return NULL;
    }
    
    // Get pointers to the data in the numpy arrays.
    double *X = (double*)PyArray_DATA(X_a);
    double *Y = (double*)PyArray_DATA(Y_a);
    double *pwise_dist = (double*)PyArray_DATA(pwise_dist_a);
    double *pwise_distYY = (double*)PyArray_DATA(pwise_distYY_a);
    double *Gamma = (double*)PyArray_DATA(Gamma_a);
    double *CDE_term = (double*)PyArray_DATA(CDE_term_a);
    double *w = (double*)PyArray_DATA(w_a);
    double *F_t = (double*)PyArray_DATA(F_t_a);
    double *wF_t = (double*)PyArray_DATA(wF_t_a);
    double *wF_t_sum = (double*)PyArray_DATA(wF_t_sum_a);
    double *p = (double*)PyArray_DATA(p_a);
    double *u = (double*)PyArray_DATA(u_a);
    int32_t *Match = (int32_t*)PyArray_DATA(Match_a);
    double *hatP = (double*)PyArray_DATA(hatP_a);
    double *hatPI_diag = (double*)PyArray_DATA(hatPI_diag_a);
    double *hatPIG = (double*)PyArray_DATA(hatPIG_a);
    double *hatPX = (double*)PyArray_DATA(hatPX_a);
    double *hatPIY = (double*)PyArray_DATA(hatPIY_a);
    double *G = (double*)PyArray_DATA(G_a);
    double *W = (double*)PyArray_DATA(W_a);
    double *GW = (double*)PyArray_DATA(GW_a);
    double *sumPoverN = (double*)PyArray_DATA(sumPoverN_a);
    double *expAlphaSumPoverN = (double*)PyArray_DATA(expAlphaSumPoverN_a);
    
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    // Actual C code
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    
    Py_BEGIN_ALLOW_THREADS
    
    dsmm::_dsmm(X,Y,M,N,D,beta,lambda,neighbor_cutoff,alpha,gamma0,
           conv_epsilon,eq_tol,
           pwise_dist,pwise_distYY,Gamma,CDE_term,
           w,F_t,wF_t,wF_t_sum,p,u,Match,
           hatP,hatPI_diag,hatPIG,hatPX,hatPIY,
           G,W,GW,sumPoverN,expAlphaSumPoverN);
           
    Py_END_ALLOW_THREADS
    
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    // End of C code
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    
    
    // Decrease the reference count for the python objects that have been 
    // declared in this function.
    Py_XDECREF(X_a);
    Py_XDECREF(Y_a);
    Py_XDECREF(pwise_dist_a);
    Py_XDECREF(pwise_distYY_a);
    Py_XDECREF(Gamma_a);
    Py_XDECREF(CDE_term_a);
    Py_XDECREF(w_a);
    Py_XDECREF(F_t_a);
    Py_XDECREF(wF_t_a);
    Py_XDECREF(wF_t_sum_a);
    Py_XDECREF(p_a);
    Py_XDECREF(u_a);
    Py_XDECREF(Match_a);
    Py_XDECREF(hatP_a);
    Py_XDECREF(hatPI_diag_a);
    Py_XDECREF(hatPIG_a);
    Py_XDECREF(hatPX_a);
    Py_XDECREF(hatPIY_a);
    Py_XDECREF(G_a);
    Py_XDECREF(W_a);
    Py_XDECREF(GW_a);
    Py_XDECREF(sumPoverN_a);
    Py_XDECREF(expAlphaSumPoverN_a);
    
    // Return the python object none. Its reference count has to be increased.
    Py_INCREF(Py_None);
    return Py_None;
}
