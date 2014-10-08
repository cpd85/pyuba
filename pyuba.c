#include <Python.h>

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

typedef struct {
  int *array; /*For future circular buffer use*/
  int *start;
  int *end;
  int numitems;
  int size;
} pyuba_struct;

static PyObject *
pyuba_getitem(PyObject *self, PyObject *args)
{
  int index;
  pyuba_struct *pyuba_str = NULL;
  PyObject *str_holder;
  if (PyArg_ParseTuple(args, "Oi", &str_holder, &index) == 0){
    return NULL;
  }
  pyuba_str = PyCapsule_GetPointer(str_holder, "");
  return Py_BuildValue("i", pyuba_str->array[index]);
}


static PyObject *
pyuba_remove(PyObject *self, PyObject *args)
{
  int index, i;
  PyObject *str_holder;
  pyuba_struct *pyuba_str;
  if (PyArg_ParseTuple(args, "Oi", &str_holder, &index) == 0) {
    return NULL;
  }
  pyuba_str = PyCapsule_GetPointer(str_holder, "");
  for (i = index; i < (pyuba_str->numitems -1); i++){
    pyuba_str->array[i] = pyuba_str->array[i + 1];
  }
  pyuba_str->numitems--;
  pyuba_str->end--;
  return Py_None;
}

static PyObject *
pyuba_insert(PyObject *self, PyObject *args)
{
  int elem, index, i;
  pyuba_struct *pyuba_ptr;
  PyObject *str_holder;
  
  if (PyArg_ParseTuple(args, "Oii" , &str_holder, &index, &elem) == 0){
    return NULL;
  }
  pyuba_ptr = PyCapsule_GetPointer(str_holder, "");;
  if (index <= pyuba_ptr->numitems && pyuba_ptr->numitems < pyuba_ptr->size) {
    pyuba_ptr->numitems++;
    for (i = pyuba_ptr->numitems - 1; i > index; i--){
      pyuba_ptr->array[i] = pyuba_ptr->array[i-1];
    }
    pyuba_ptr->end++;
    pyuba_ptr->array[index] = elem;
    return Py_None;
  } else {
    /* We need to extend !!! */
    int *newarray = malloc(sizeof(int) * MAX(index, 2*pyuba_ptr->numitems));
    (void) memcpy(newarray, pyuba_ptr->array, 
          (sizeof(int) * (pyuba_ptr->numitems)));
    (void) free(pyuba_ptr->array);
    pyuba_ptr->array = newarray;
    pyuba_ptr->start = newarray;
    pyuba_ptr->size = MAX(index, 2*pyuba_ptr->numitems);
    pyuba_ptr->numitems++;
    pyuba_ptr->array[index] = elem;
    return Py_None;
  }

}

static PyObject *
pyuba_append(PyObject *self, PyObject *args)
{
  int elem;
  PyObject *str_holder;
  int *newarray;
  pyuba_struct *pyuba_str;
  if (PyArg_ParseTuple(args, "li", &str_holder, &elem) == 0) {
    return NULL;
  }
  pyuba_str = PyCapsule_GetPointer(str_holder, "");
  if (++(pyuba_str->numitems) == pyuba_str->size){
    /* We need to double the array size before insertion */
    newarray = malloc(2 * sizeof(int) * (pyuba_str->numitems));
    (void) memcpy(newarray, pyuba_str->array, 
                   (pyuba_str->numitems * sizeof(int)));
    (void) free(pyuba_str->array);
    pyuba_str->array = newarray;
    pyuba_str->start = newarray;
    pyuba_str->end = newarray + pyuba_str->numitems;
    pyuba_str->size *= 2;
  } else {
    pyuba_str->end++;
  }

  
  pyuba_str->array[pyuba_str->numitems - 1] = elem;

  return Py_None;
}

static PyObject *
new_pyuba_method(PyObject *self, PyObject *args)
{
  int size;
  int i;
  PyObject *retcapsule;
  if (PyArg_ParseTuple(args, "i", &size) == 0) {
    return NULL;
  }
  /* Prepare pyuba structure */
  pyuba_struct *mystruct = malloc(sizeof(pyuba_struct));
  mystruct->array = malloc(sizeof(int) * size);
  for (i = 0; i < size; i++){
    mystruct->array[i] = 0;
  }
  mystruct->size = size;
  mystruct->numitems = 0;
  mystruct->start = mystruct->array;
  mystruct->end = mystruct->start;
  retcapsule = PyCapsule_New(mystruct, "", NULL); 
  return retcapsule;
}

static PyMethodDef pyuba_methods[] = {
  {"insert", pyuba_insert, METH_VARARGS},
  {"remove", pyuba_remove, METH_VARARGS},
  {"append", pyuba_append, METH_VARARGS},
  {"new_pyuba", new_pyuba_method, METH_VARARGS},
  {"getitem", pyuba_getitem, METH_VARARGS}, 
  {NULL, NULL}
};

void
initpyuba()
{
  (void) Py_InitModule("pyuba", pyuba_methods);
}
