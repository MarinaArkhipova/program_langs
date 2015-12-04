#include <vector>

extern "C" {
#include <Python.h>
}


namespace matrixops {

	typedef std::vector<double> row_t;
	typedef std::vector<row_t> matrix_t;

	static matrix_t floydWarsh(matrix_t &matrix)
	{
		for (size_t k = 0; k < matrix.size(); k++) {
			for (size_t i = 0; i < matrix.size(); i++) {
				for (size_t j = 0; j < matrix.size(); j++) {
					double temp = matrix[i][k] + matrix[k][j];
					if (matrix[i][j] != 0 && temp != 0) {
						matrix[i][j] = 1.0 * temp*matrix[i][j]/(temp + matrix[i][j]);
					}
				}
			}
		}
		return matrix;
	}
}

static matrixops::matrix_t pyobject_to_cxx(PyObject * py_matrix)
{
	matrixops::matrix_t result;
	result.resize(PyObject_Length(py_matrix));
	for (size_t i=0; i<result.size(); ++i) {
		PyObject * py_row = PyList_GetItem(py_matrix, i);
		matrixops::row_t & row = result[i];
		row.resize(PyObject_Length(py_row));
		for (size_t j=0; j<row.size(); ++j) {
			PyObject * py_elem = PyList_GetItem(py_row, j);
			const double elem = PyFloat_AsDouble(py_elem);
			row[j] = elem;
		}
	}
	return result;
}

static PyObject * cxx_to_pyobject(const matrixops::matrix_t &matrix)
{
	PyObject * result = PyList_New(matrix.size());
	for (size_t i=0; i<matrix.size(); ++i) {
		const matrixops::row_t & row = matrix[i];
		PyObject * py_row = PyList_New(row.size());
		PyList_SetItem(result, i, py_row);
		for (size_t j=0; j<row.size(); ++j) {
			const double elem = row[j];
			PyObject * py_elem = PyFloat_FromDouble(elem);
			PyList_SetItem(py_row, j, py_elem);
		}
	}
	return result;
}

static PyObject * matrix_operations(PyObject * module, PyObject * args)
{
	PyObject * matrix = PyTuple_GetItem(args, 0);

	/* Convert to C++ structure */
	matrixops::matrix_t matrix_cxx = pyobject_to_cxx(matrix);

	/* Perform calculations */
	matrixops::matrix_t result = matrixops::floydWarsh(matrix_cxx);

	/* Convert back to Python object */
	PyObject * py_result = cxx_to_pyobject(result);
	return py_result;
}


PyMODINIT_FUNC PyInit_matrixops()
{
	static PyMethodDef ModuleMethods[] = {
		{ "applyFloydWarshal", matrix_operations, METH_VARARGS, "Floyd Warsh algorythm" },
		{ NULL, NULL, 0, NULL }
	};
	static PyModuleDef ModuleDef = {
		PyModuleDef_HEAD_INIT,
		"matrixops",
		"FloydWarshall algorythm",
		-1, ModuleMethods,
		NULL, NULL, NULL, NULL
	};
	PyObject * module = PyModule_Create(&ModuleDef);
	return module;
}

