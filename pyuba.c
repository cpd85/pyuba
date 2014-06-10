#include <Python/Python.h>

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
  unsigned long structptr;
  pyuba_struct *pyuba_str;
  if (PyArg_ParseTuple(args, "li", &structptr, &index) == 0){
    return NULL;
  }
  pyuba_str = (pyuba_struct *) structptr;
  return Py_BuildValue("i", pyuba_str->array[index]);
}
static PyObject *
pyuba_remove(PyObject *self, PyObject *args)
{
  int index, i;
  unsigned long structptr;
  pyuba_struct *pyuba_str;
  if (PyArg_ParseTuple(args, "li", &structptr, &index) == 0) {
    return NULL;
  }
  pyuba_str = (pyuba_struct *) structptr;
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
  int elem, index;
  unsigned long structptr;
  int *newarray;
  pyuba_struct *pyuba_str;
  if (PyArg_ParseTuple(args, "lii", &structptr, &index, &elem) == 0) {
    return NULL;
  }
  pyuba_str = (pyuba_struct *) structptr;
  if (++(pyuba_str->numitems) == pyuba_str->size){
    /* We need to double the array size before insertion */
    newarray = malloc(2 * sizeof(int) * (pyuba_str->numitems));
    memcpy(newarray, pyuba_str->array, (pyuba_str->numitems * sizeof(int)));
    pyuba_str->array = newarray;
    pyuba_str->start = newarray;
    pyuba_str->end = newarray + pyuba_str->numitems;
    pyuba_str->size *= 2;
  } else {
    int i;
    int *start = pyuba_str->start;
    for (i = (pyuba_str->numitems - 1); i > (index); i--){
      pyuba_str->start[i + 1] = pyuba_str->start[i];
    }
    pyuba_str->start[index] = elem;
    pyuba_str->end++;
  }
  return Py_None;
}

static PyObject *
new_pyuba_method(PyObject *self, PyObject *args)
{
  int size;

  if (PyArg_ParseTuple(args, "i", &size) == 0) {
    return NULL;
  }
  /* Prepare pyuba structure */
  pyuba_struct *mystruct = malloc(sizeof(pyuba_struct));
  mystruct->array = malloc(sizeof(int) * 2 * size);
  mystruct->size = 2 * size;
  mystruct->numitems = 0;
  mystruct->start = mystruct->array;
  mystruct->end = mystruct->end;
  
  return Py_BuildValue("l", (long) mystruct);
}

static PyMethodDef pyuba_methods[] = {
  {"insert", pyuba_insert, METH_VARARGS},
  {"remove", pyuba_remove, METH_VARARGS},
  {"new_pyuba", new_pyuba_method, METH_VARARGS},
  {"getitem", pyuba_getitem, METH_VARARGS}, 
  {NULL, NULL}
};

void
initpyuba()
{
  (void) Py_InitModule("pyuba", pyuba_methods);
}
