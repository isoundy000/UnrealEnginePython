
#include "UnrealEnginePythonPrivatePCH.h"

#include "Runtime/Slate/Public/Widgets/Views/STreeView.h"

#include "UEPySPythonTreeView.h"


#define sw_python_tree_view StaticCastSharedRef<SPythonTreeView>(self->s_tree_view.s_list_view.s_table_view_base.s_compound_widget.s_widget.s_widget)


static PyMethodDef ue_PySPythonTreeView_methods[] = {
	{ NULL }  /* Sentinel */
};

PyTypeObject ue_PySPythonTreeViewType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"unreal_engine.SPythonTreeView", /* tp_name */
	sizeof(ue_PySPythonTreeView), /* tp_basicsize */
	0,                         /* tp_itemsize */
	0,       /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_reserved */
	0,                         /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash  */
	0,                         /* tp_call */
	0,                         /* tp_str */
	0,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
	"Unreal Engine SPythonTreeView",           /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	ue_PySPythonTreeView_methods,             /* tp_methods */
};

static int ue_py_spython_tree_view_init(ue_PySPythonTreeView *self, PyObject *args, PyObject *kwargs) {

	ue_py_slate_setup_farguments(SPythonTreeView);

	// first of all check for values
	PyObject *values = ue_py_dict_get_item(kwargs, "tree_items_source");
	if (!values) {
		PyErr_SetString(PyExc_Exception, "you must specify tree items");
		return -1;
	}

	values = PyObject_GetIter(values);
	if (!values) {
		PyErr_SetString(PyExc_Exception, "values field is not an iterable");
		return -1;
	}

	TArray<TSharedPtr<FPythonItem>> *items = new TArray<TSharedPtr<FPythonItem>>();
	while (PyObject *item = PyIter_Next(values)) {
		Py_INCREF(item);
		// keep track of items
		self->s_tree_view.s_list_view.s_table_view_base.s_compound_widget.s_widget.py_refs.Add(item);
		items->Add(TSharedPtr<FPythonItem>(new FPythonItem(item)));
	}
	Py_DECREF(values);

	arguments.TreeItemsSource(items);

	ue_py_slate_farguments_optional_enum("allow_overscroll", AllowOverscroll, EAllowOverscroll);
	ue_py_slate_farguments_optional_bool("clear_selection_on_click", ClearSelectionOnClick);
	ue_py_slate_farguments_optional_enum("consume_mouse_wheel", ConsumeMouseWheel, EConsumeMouseWheel);
	ue_py_slate_farguments_float("item_height", ItemHeight);
	ue_py_slate_farguments_event("on_generate_row", OnGenerateRow, TSlateDelegates<TSharedPtr<FPythonItem>>::FOnGenerateRow, GenerateRow);
	ue_py_slate_farguments_event("on_selection_changed", OnSelectionChanged, TSlateDelegates<TSharedPtr<FPythonItem>>::FOnSelectionChanged, OnSelectionChanged);
	ue_py_slate_farguments_enum("selection_mode", SelectionMode, ESelectionMode::Type);
#if ENGINE_MINOR_VERSION > 12
	ue_py_slate_farguments_optional_float("wheel_scroll_multiplier", WheelScrollMultiplier);
#endif
	ue_py_slate_farguments_event("on_get_children", OnGetChildren, TSlateDelegates<TSharedPtr<FPythonItem>>::FOnGetChildren, GetChildren);

	ue_py_snew(SPythonTreeView, s_tree_view.s_list_view.s_table_view_base.s_compound_widget.s_widget);
	return 0;
}

void ue_python_init_spython_tree_view(PyObject *ue_module) {

	ue_PySPythonTreeViewType.tp_init = (initproc)ue_py_spython_tree_view_init;

	ue_PySPythonTreeViewType.tp_base = &ue_PySTreeViewType;

	if (PyType_Ready(&ue_PySPythonTreeViewType) < 0)
		return;

	Py_INCREF(&ue_PySTreeViewType);
	PyModule_AddObject(ue_module, "SPythonTreeView", (PyObject *)&ue_PySPythonTreeViewType);
}
