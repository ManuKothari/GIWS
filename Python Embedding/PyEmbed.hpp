#ifndef __PYTHONEMBED__H
#define __PYTHONEMBED__H

#include <bits/stdc++.h>
#include <python2.7/Python.h>  //Will have to change later -> Don't forget!


class Python {
private:
	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pArgs, *pValue, *pClass , *pInstance;
	void init() {
		setenv("PYTHONPATH",".",0);
		pValue = pArgs = pFunc = pDict = pModule = pName = pClass = 0;
		Py_Initialize();
	}
protected:

public:
	explicit Python() {
		init();
	}
	explicit Python(const char *progName) {
		init();
		pName = PyString_FromString(progName);
		pModule = PyImport_Import(pName);
		pDict = PyModule_GetDict(pModule);
		pName = 0;
	}
	virtual ~Python() {
		Py_DECREF(pName);
		Py_DECREF(pModule);
		Py_DECREF(pDict);
		Py_DECREF(pFunc);
		Py_DECREF(pArgs);
		Py_DECREF(pValue);
		Py_Finalize();

	}
	void callFunction(const char *functionName, int nCount = 0, ...) {
		if (pModule) {
			pFunc = PyDict_GetItemString(pDict, functionName);
			if(!pFunc) {
				PyErr_Print();
				std::cerr << "Failed to load " << functionName << std::endl;
				return;
			}
		}
		if (PyCallable_Check(pFunc)) {
			if(nCount > 0) {
				va_list list;
				va_start(list, nCount);
				pArgs = PyTuple_New(nCount);
				for (int i=0; i < nCount; ++i) {
					pValue = PyInt_FromLong(va_arg(list,long));
					if (!pValue) {
						fprintf(stderr, "Cannot convert argument\n");
						return;
					}
					PyTuple_SetItem(pArgs, i, pValue);
				}
			}
			pValue = PyObject_CallObject(pFunc, pArgs);
			if (pValue != NULL)	{
				std::cout << "Return of call :" << PyInt_AsLong(pValue) << std::endl;
			}
			else {
				PyErr_Print();
			}
		}
		else {
			PyErr_Print();
		}
	}
};

#endif
