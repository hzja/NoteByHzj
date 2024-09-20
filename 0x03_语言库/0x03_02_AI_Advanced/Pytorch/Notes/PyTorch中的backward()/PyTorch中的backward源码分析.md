PyTorch<br />说起backward大家肯定不陌生，用过**PyTorch**的肯定都知道，这个函数的作用是反向传播计算梯度的。比如下边这个例子，**要反向传播计算梯度之后**，**才能调用优化器的**`**step**`**函数更新网络模型参数**。
```python
Example:
>>> optimizer = torch.optim.SGD(model.parameters(), lr=0.1, momentum=0.9)
>>> optimizer.zero_grad()
>>> loss_fn(model(input), target).backward()
>>> optimizer.step()
```
<a name="tp3gL"></a>
## [1] `torch.Tensor.backward`
在 torch/tensor.py 文件中可以看到，`**class Tensor(torch._C._TensorBase)**`中有函数`**def backward**`。所以可以用`**tensor.backward()**`来进行反向传播。
```python
# https://pytorch.org/docs/stable/generated/torch.Tensor.backward.html

def backward(self, gradient=None, retain_graph=None, create_graph=False):
    r"""Computes the gradient of current tensor w.r.t. graph leaves.

    The graph is differentiated using the chain rule. If the tensor is
    non-scalar (i.e. its data has more than one element) and requires
    gradient, the function additionally requires specifying ``gradient``.
    It should be a tensor of matching type and location, that contains
    the gradient of the differentiated function w.r.t. ``self``.

    This function accumulates gradients in the leaves - you might need to
    zero them before calling it.

    Arguments:
        gradient (Tensor or None): Gradient w.r.t. the
            tensor. If it is a tensor, it will be automatically converted
            to a Tensor that does not require grad unless ``create_graph`` is True.
            None values can be specified for scalar Tensors or ones that
            don't require grad. If a None value would be acceptable then
            this argument is optional.
        retain_graph (bool, optional): If ``False``, the graph used to compute
            the grads will be freed. Note that in nearly all cases setting
            this option to True is not needed and often can be worked around
            in a much more efficient way. Defaults to the value of
            ``create_graph``.
        create_graph (bool, optional): If ``True``, graph of the derivative will
            be constructed, allowing to compute higher order derivative
            products. Defaults to ``False``.
    """
    torch.autograd.backward(self, gradient, retain_graph, create_graph)
```
其中，`**create_graph**`参数的作用是，如果为**True**，那么就**创建一个专门的graph of the derivative**，这可以方便计算高阶微分。参数`**retain_graph**`可以忽略，因为绝大多数情况根本不需要，它的**作用是要不要保留Graph**。该函数实现代码也很简单，就是调用`**torch.autograd.backward**`。所以接下来看一下`**torch.autograd.backward**`中的实现。
<a name="G27uR"></a>
## [2] `torch.autograd.backward`
函数`**torch.autograd.backward**`的定义在文件 torch/autograd/__init__.py 中。借助于**链式法则the chain rule**和**Jacobian-vector product**可以很方便的计算梯度。下边就是具体的代码。
```python
# https://github.com/pytorch/pytorch/blob/master/torch/autograd/__init__.py
# https://pytorch.org/docs/stable/generated/torch.autograd.backward.html
# ...

from .variable import Variable

# ... 

def _make_grads(outputs, grads):
    new_grads = []
    for out, grad in zip(outputs, grads):
        if isinstance(grad, torch.Tensor):
            if not out.shape == grad.shape:
                # raise RuntimeError ...
            new_grads.append(grad)
        elif grad is None:
            if out.requires_grad:
                if out.numel() != 1:
                    # raise RuntimeError ...
            else:
                new_grads.append(None)
        else:
            # raise TypeError ...
    return tuple(new_grads)


def backward(tensors, grad_tensors=None, retain_graph=None, create_graph=False, grad_variables=None):
    r"""Computes the sum of gradients of given tensors w.r.t. graph leaves.

    The graph is differentiated using the chain rule. If any of ``tensors``
    are non-scalar (i.e. their data has more than one element) and require
    gradient, then the Jacobian-vector product would be computed, in this
    case the function additionally requires specifying ``grad_tensors``.
    It should be a sequence of matching length, that contains the "vector"
    in the Jacobian-vector product, usually the gradient of the differentiated
    function w.r.t. corresponding tensors (``None`` is an acceptable value for
    all tensors that don't need gradient tensors).

    This function accumulates gradients in the leaves - you might need to zero
    them before calling it.
    """
    if grad_variables is not None:
        warnings.warn("'grad_variables' is deprecated. Use 'grad_tensors' instead.")
        if grad_tensors is None:
            grad_tensors = grad_variables
        else:
            raise RuntimeError("'grad_tensors' and 'grad_variables' (deprecated) "
                               "arguments both passed to backward(). Please only "
                               "use 'grad_tensors'.")

    tensors = (tensors,) if isinstance(tensors, torch.Tensor) else tuple(tensors)

    if grad_tensors is None:
        grad_tensors = [None] * len(tensors)
    elif isinstance(grad_tensors, torch.Tensor):
        grad_tensors = [grad_tensors]
    else:
        grad_tensors = list(grad_tensors)

    grad_tensors = _make_grads(tensors, grad_tensors)
    if retain_graph is None:
        retain_graph = create_graph

    Variable._execution_engine.run_backward(
        tensors, grad_tensors, retain_graph, create_graph,
        allow_unreachable=True)  # allow_unreachable flag

# ...

if not torch._C._autograd_init():
    raise RuntimeError("autograd initialization failed")
```
参数`**grad_variables**`是老版本的，已经被`**deprecated**`，现在使用的是`**grad_tensors**`。即便使用了也没关系，代码会把参数**grad_variables**的值传给参数`**grad_tensors**`以供使用。代码中用到了函数`**_make_grads**`，该函数主要是对**grad_tensors**中的元素进行检查并且将`**grad_tensors**`重新组织成`**tuple(list(torch.Tensor, ...))**`的形式。做完这一系列操作之后就是调用`**Variable._execution_engine.run_backward**`，并且将这些被`**check**`和重新组织的参数传给该函数。注意参数`**allow_unreachable**`，下边还会遇到。
<a name="Bmf4c"></a>
## [3] `Variable._execution_engine.run_backward`
从文件中的代码`**from .variable import Variable**`可以知道，`**Variable**`的定义在文件 torch/autograd/variable.py 中。具体代码如下。
```cpp
# https://github.com/pytorch/pytorch/blob/master/torch/autograd/variable.py

import torch
from torch._six import with_metaclass


class VariableMeta(type):
    def __instancecheck__(cls, other):
        return isinstance(other, torch.Tensor)


class Variable(with_metaclass(VariableMeta, torch._C._LegacyVariableBase)):
    pass


from torch._C import _ImperativeEngine as ImperativeEngine
Variable._execution_engine = ImperativeEngine()

# https://github.com/pytorch/pytorch/tree/master/torch/csrc
```
代码内容很短，可以看到，前边看到的函数`**Variable._execution_engine.run_backward**`其实就是**torch._C**中的函数`**_ImperativeEngine**`。**torch._C**这个是调用的被编译之后的**C++**代码，**Windows**系统下可以在**Python**目录**\Lib\site-packages\torch**下找到**_C.cp35-win_amd64.pyd**这个文件，当然不同的**Python**版本名称也会略有不同，但是这个**_C**和**.pyd**是一样的。具体的函数实现代码可以从**GitHub**上 pytorch/torch/csrc 这里找到。
<a name="VVw2J"></a>
## [4] `torch._C._ImperativeEngine`
很容易就可以找到，函数`_ImperativeEngine`在文件 torch/csrc/autograd/python_engine.cpp 中的第 308 行出现。代码如下。
```cpp
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/python_engine.cpp#L308

bool THPEngine_initModule(PyObject *module)
{
#ifndef _WIN32
  if (pthread_atfork(nullptr, nullptr, child_atfork) != 0) {
    throw std::runtime_error("unable to set pthread_atfork handler");
  }
#endif
  if (PyType_Ready(&THPEngineType) < 0)
    return false;
  Py_INCREF(&THPEngineType);
  PyModule_AddObject(module, "_ImperativeEngine", (PyObject *)&THPEngineType);
  set_default_engine_stub(get_python_engine);
  return true;
}

// https://github.com/pytorch/pytorch/blob/master/torch/csrc/Module.cpp%23L679
```
通过函数`**PyModule_AddObject**`将`**(PyObject *)&THPEngineType**`这个**object**加入到模块**module**中并命名为`**_ImperativeEngine**`。这个`**module**`的类型是`**PyObject**`，这个初始化函数可以在文件 torch/csrc/Module.cpp() 的第 679 行找到，`**module**`的定义则是在第 67 行。<br />关于函数`**PyModule_AddObject**`的详细介绍可以参考[https://docs.python.org/3.5/c-api/module.html](https://docs.python.org/3.5/c-api/module.html) 。<br />另外关于 Python 扩展的相关知识，可以参考[https://docs.python.org/3.5/extending/index.html](https://docs.python.org/3.5/extending/index.html)。<br />现在回过头来看之前的`Variable._execution_engine.run_backward()`其实就是`_ImperativeEngine().run_backward()`。从对象`THPEngineType`的定义可以找到`run_backward`也只是个外套，具体的C++函数其实是`THPEngine_run_backward`。这部分代码仍然是在 torch/csrc/autograd/python_engine.cpp 中。
```cpp
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/python_engine.cpp#L308
// ...
 
static struct PyMethodDef THPEngine_methods[] = {
  {(char*)"run_backward", (PyCFunction)(void(*)(void))THPEngine_run_backward, METH_VARARGS | METH_KEYWORDS, nullptr},
  {(char*)"queue_callback", (PyCFunction)THPEngine_queue_callback, METH_O, nullptr},
  {(char*)"is_checkpoint_valid", (PyCFunction)THPEngine_is_checkpoint_valid, METH_NOARGS, nullptr},
  {nullptr}
};

// ...

PyTypeObject THPEngineType = {
    // ...
    "torch._C._EngineBase",                      /* tp_name */
    // ...
    THPEngine_methods,                           /* tp_methods */
    // ...
};

// ...
```
代码中使用了`**PyMethodDef**`，该函数是用于描述扩展方法的`**struct**`。可以看到，除了要找的函数`**run_backward**`，此处还定义了函数`**queue_callback**`和函数`**is_checkpoint_valid**`。
<a name="s6e3v"></a>
## [5] `THPEngine_run_backward`
关于函数`**THPEngine_run_backward**`的介绍是`**Implementation of torch._C._EngineBase.run_backward**`，而`**torch._C._EngineBase**`这个名字在`**THPEngineType**`的定义部分的代码可以找到。该部分代码超过一百行了，下边分块来看一下。<br />首先把中间部分略去。函数内第一行和最后一行的`**HANDLE_TH_ERRORS**`和`**END_HANDLE_TH_ERRORS**`，是在文件 torch/csrc/Exceptions.h 中定义的宏，具体地，分别在第 41 行和第 114 行被定义。这部分代码主要是通过函数`**PyArg_ParseTupleAndKeywords**`对输入的参数重新解析并赋值给新定义的变量`**tensors**`、`**grad_tensors**`、`**keep_graph**`、`**create_graph**`、`**inputs**`以及`**allow_unreachable**`。<br />有关函数`**PyArg_ParseTupleAndKeywords**`的用法详见 [https://docs.python.org/3.5/c-api/arg.html#c.PyArg_ParseTupleAndKeywords](https://docs.python.org/3.5/c-api/arg.html#c.PyArg_ParseTupleAndKeywords) 。
```cpp
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/Exceptions.h
// Implementation of torch._C._EngineBase.run_backward
PyObject *THPEngine_run_backward(THPEngine *self, PyObject *args, PyObject *kwargs)
{
    HANDLE_TH_ERRORS
    _maybe_reinitialize_engine_after_fork();
    PyObject *tensors = nullptr;
    PyObject *grad_tensors = nullptr;
    unsigned char keep_graph = 0;
    unsigned char create_graph = 0;
    PyObject *inputs = nullptr;
    unsigned char allow_unreachable = 0;
    const char *accepted_kwargs[] = {
            "tensors", "grad_tensors", "keep_graph", "create_graph", "inputs",
            "allow_unreachable", nullptr
    };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OObb|Ob", (char**)accepted_kwargs,
                &tensors, &grad_tensors, &keep_graph, &create_graph, &inputs, &allow_unreachable))
        return nullptr;

    // ...

    END_HANDLE_TH_ERRORS
}
```
下面来看下中间的部分。这部分主要是`**Check**`一下`**tensors**`和`**grad_tensors**`的变量类型，并且检查二者的`**tuple size**`是否一致。
```cpp
// Implementation of torch._C._EngineBase.run_backward
PyObject *THPEngine_run_backward(THPEngine *self, PyObject *args, PyObject *kwargs)
{
    // ...

    THPUtils_assert(PyTuple_Check(tensors), "tensors argument is expected to "
            "be a tuple, but got %s", THPUtils_typename(tensors));
    THPUtils_assert(PyTuple_Check(grad_tensors), "grad_tensors argument is "
            "expected to be a tuple, but got %s", THPUtils_typename(grad_tensors));

    Py_ssize_t num_tensors = PyTuple_GET_SIZE(tensors);
    Py_ssize_t num_gradients = PyTuple_GET_SIZE(grad_tensors);
    THPUtils_assert(num_tensors == num_gradients, "got %ld tensors and %ld "
            "gradients", num_tensors, num_gradients);

    // ...
}
```
下边这部分代码也比较简单。先是定义`**edge_list roots;**`和`**variable_list grads;**`。接下来通过循环把`**tensors**`和`**grad_tensors**`中的元素`**push_back**`进`**roots**`和`**grads**`。具体地，先通过`**PyTuple_GET_ITEM**`取出元素，再利用`**((THPVariable*)···)->cdata**`取出元素的值。当然中间也会做一些`**Check**`，例如是否为`**Tensor**`之类的。
```cpp
// Implementation of torch._C._EngineBase.run_backward
PyObject *THPEngine_run_backward(THPEngine *self, PyObject *args, PyObject *kwargs)
{
    // ...

    edge_list roots;
    roots.reserve(num_tensors);
    variable_list grads;
    grads.reserve(num_tensors);
    for (int i = 0; i < num_tensors; i++) {
        PyObject *_tensor = PyTuple_GET_ITEM(tensors, i);
        THPUtils_assert(THPVariable_Check(_tensor), "element %d of tensors "
                "tuple is not a Tensor", i);
        auto& variable = ((THPVariable*)_tensor)->cdata;
        auto gradient_edge = torch::autograd::impl::gradient_edge(variable);
        THPUtils_assert(gradient_edge.function,
                "element %d of tensors does not require grad and does not have a grad_fn", i);
        roots.push_back(std::move(gradient_edge));

        PyObject *grad = PyTuple_GET_ITEM(grad_tensors, i);
        if (THPVariable_Check(grad)) {
            const Variable& grad_var = ((THPVariable*)grad)->cdata;
            if (grad_var.has_names()) {
                TORCH_WARN(
                        "Autograd was passed a named grad tensor with dims ", grad_var.names(),
                        ". Autograd does not yet support named tensor semantics, so all names ",
                        "will be ignored. In practice all computed gradients will still be correct "
                        "according to regular tensor semantics.");
            }
            grads.push_back(grad_var);
        } else {
            THPUtils_assert(grad == Py_None,
                    "element %d of gradients tuple is not a Tensor or None", i);
            THPUtils_assert(!variable.requires_grad(),
                    "element %d of gradients tuple is None, but the corresponding Tensor requires grad");
        }
    }

    // ...
}
```
下边继续看。这部分代码就是对`**inputs**`中的每一个元素都传入函数`**torch::autograd::impl::try_get_grad_accumulator**`中去处理。函数`**try_get_grad_accumulator**`被定义在文件 torch/csrc/autograd/variable.h 的第 113 行，具体实现则是在文件 torch/csrc/autograd/variable.cpp 的第111 行，这个等下再说，现在只需要知道返回的是个指向`**Node**`对象的指针。接下来就是，如果指针不是空指针，则执行`**output_edges.emplace_back(grad_fn, output_nr)**`。<br />函数`**push_back()**`和`**emplace_back()**`的区别是，`**push_back()**`函数向容器中加入一个临时对象（右值元素）时， 首先会调用构造函数生成这个对象，然后条用拷贝构造函数将这个对象放入容器中， 最后释放临时对象。但是**emplace_back()**函数向容器中中加入临时对象， 临时对象原地构造，没有赋值或移动的操作。详细内容参阅 cpp/container/vector/emplace_back 。
```cpp
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/variable.h#L113
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/variable.cpp#L111
// https://en.cppreference.com/w/cpp/container/vector/emplace_back

// Implementation of torch._C._EngineBase.run_backward
PyObject *THPEngine_run_backward(THPEngine *self, PyObject *args, PyObject *kwargs)
{
    // ...

    std::vector<Edge> output_edges;
    if (inputs != nullptr) {
        int num_inputs = PyTuple_GET_SIZE(inputs);
        output_edges.reserve(num_inputs);
        for (int i = 0; i < num_inputs; ++i) {
            PyObject *input = PyTuple_GET_ITEM(inputs, i);
            THPUtils_assert(THPVariable_Check(input),
                    "all inputs have to be Tensors, but got %s", THPUtils_typename(input));
            THPVariable *input_var = (THPVariable*)input;
            const auto output_nr = input_var->cdata.output_nr();
            auto grad_fn = input_var->cdata.grad_fn();
            if (!grad_fn) {
                    grad_fn = torch::autograd::impl::try_get_grad_accumulator(input_var->cdata);
            }
            THPUtils_assert(input_var->cdata.requires_grad(),
                    "One of the differentiated Tensors does not require grad");
            if (!grad_fn) {
                output_edges.emplace_back();
            } else {
                output_edges.emplace_back(grad_fn, output_nr);
            }
        }
    }

    // ...
}
```
现在来看看传入`**output_edges**`的这两个参数都是什么类型。`**grad_fn**`是指向Node对象的**std::shared_ptr**指针，现在来看看另外一个参数`**output_nr**`。结构体`**THPVariable**`被定义在文件 torch/csrc/autograd/python_variable.h 中，代码如下所示。可以看到其中`**cdata**`变量的类型是`**torch::autograd::Variable**`。最终在 torch/csrc/autograd/VariableTypeManual.cpp 找到函数`**output_nr**`，其返回的是文件 torch/csrc/autograd/variable.h 中定义的结构体`**AutogradMeta**`中的成员变量`**uint32_t output_nr_;**`，这和文件 torch/csrc/autograd/edge.h 中定义的结构体Edge初始化的参数类型刚好吻合。
```cpp
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/python_variable.h#L12
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/VariableTypeManual.cpp#L134
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/variable.h#L179
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/edge.h#L14

// python_variable.h

// Python object that backs torch.autograd.Variable
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
struct THPVariable {
    PyObject_HEAD
    // Payload
    torch::autograd::Variable cdata;
    // Hooks to be run on backwards pass (corresponds to Python attr
    // '_backwards_hooks', set by 'register_hook')
    PyObject* backward_hooks = nullptr;
};

// =======================================================

// VariableTypeManual.cpp

int64_t output_nr(const Tensor & self) {
    if (impl::get_autograd_meta(self)) {
        return impl::get_autograd_meta(self)->output_nr_;
    } else {
        return 0;
    }
}

// =======================================================

// variable.h

struct TORCH_API AutogradMeta : public c10::AutogradMetaInterface {
    // ...
    // The "output number" of this variable; e.g., if this variable
    // was the second output of a function, then output_nr == 1.
    // We use this to make sure we can setup the backwards trace
    // correctly when this variable is passed to another function.
    uint32_t output_nr_;
    // ...
};

// =======================================================

// edge.h

/// Represents a particular input of a function.
struct Edge {
    Edge() noexcept : function(nullptr), input_nr(0) {}
    Edge(std::shared_ptr<Node> function_, uint32_t input_nr_) noexcept
        : function(std::move(function_)), input_nr(input_nr_) {}
    // ...
};
```
该函数最后这部分的代码如下所示。注意到`**THPUtils_assert(allow_unreachable ... );**`，其中`**allow_unreachable flag**`可以追溯到上边第二部分的源码，通过`**Variable._execution_engine.run_backward**`传入的是`**allow_unreachable=True**`。`**PyTuple_GET_SIZE**`的作用是获取传入参数的**size**；`**PyTuple_New**`的作用是创建一个新的`**tuple**`对象，传入的参数就是新的`**tuple**`对象的`**size**`；`**PyTuple_SET_ITEM**`的作用是将`**THPVariable_Wrap(outputs[i])**`传入到`**py_outputs.get()**`的位置**i**处。这里最关键的就是函数`**engine.execute**`，下边具体介绍。
```cpp
// Implementation of torch._C._EngineBase.run_backward
PyObject *THPEngine_run_backward(THPEngine *self, PyObject *args, PyObject *kwargs)
{
    // ...

    variable_list outputs;
    {
        pybind11::gil_scoped_release no_gil;
        outputs = engine.execute(roots, grads, keep_graph, create_graph, output_edges);
    }

    if (inputs != nullptr) {
        int num_inputs = PyTuple_GET_SIZE(inputs);
        THPObjectPtr py_outputs {PyTuple_New(num_inputs)};
        if (!py_outputs) return nullptr;
        for (int i = 0; i < num_inputs; i++) {
            THPUtils_assert(allow_unreachable || outputs[i].defined(), "One of the "
                                            "differentiated Tensors appears to not have been used "
                                            "in the graph. Set allow_unused=True if this is the "
                                            "desired behavior.");
            PyTuple_SET_ITEM(py_outputs.get(), i, THPVariable_Wrap(outputs[i]));
        }
        return py_outputs.release();
    } else {
        Py_RETURN_NONE;
    }

    // ...
}
```
像`**PyTuple_GET_SIZE**`、`**PyTuple_New**`以及`**PyTuple_SET_ITEM**`这样的函数，用处就是可以在**C++**中操纵**Python**对象。关于`**tuple**`的类似的函数可以查阅 [https://docs.python.org/3.5/c-api/tuple.html](https://docs.python.org/3.5/c-api/tuple.html) 。这些其实也很好记，函数名带有**PyTuple**的就是**Python**的**Tuple**对象，带有**PyList**的就是**Python**的**List**对象，带有**PyType**的就是**Python**的**Type**对象。更多内容可以去看一下 [https://docs.python.org/3.5/c-api/concrete.html](https://docs.python.org/3.5/c-api/concrete.html) 。
<a name="LSfXG"></a>
## [6] `try_get_grad_accumulator`
现在回头来看一下函数`**try_get_grad_accumulator**`，定义在文件 torch/csrc/autograd/variable.h 的第 113 行，具体实现则是在文件 torch/csrc/autograd/variable.cpp 的第111 行。源码简化之后，如下所示。
```cpp
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/variable.h#L113
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/variable.cpp#L111

// variable.h
// ...
namespace torch { namespace autograd {
struct Node; 
struct AutogradMeta;
struct DifferentiableViewMeta;
using Variable = at::Tensor;
namespace impl {
    // ...
    TORCH_API AutogradMeta* get_autograd_meta(const Variable&);
    // ...
    TORCH_API std::shared_ptr<Node> try_get_grad_accumulator(const Variable&);
    // ...
}
// ...
struct TORCH_API AutogradMeta : public c10::AutogradMetaInterface {
    // ...
    std::weak_ptr<Node> grad_accumulator_; 
    // ...
};

// =================================

// variable.cpp
// ...
namespace torch {
namespace autograd {
// ...
namespace impl {
    // ...
    std::shared_ptr<Node> try_get_grad_accumulator(const Variable& self) {
        if (get_autograd_meta(self)) {
            return get_autograd_meta(self)->grad_accumulator_.lock();
        } else {
            return nullptr;
        }
    }
    // ...
    AutogradMeta* get_autograd_meta(const Variable& self) {
        // NB: could return null
        TORCH_CHECK(self.defined(), "cannot call get_autograd_meta() on undefined tensor");
        return static_cast<AutogradMeta*>(self.unsafeGetTensorImpl()->autograd_meta());
    }
    // ...
}
// ...
```
所以函数`**try_get_grad_accumulator**`就是先通过函数`**get_autograd_meta**`返回一个`**AutogradMeta**`结构体，然后访问结构体中的成员变量`**grad_accumulator_**`，而`**grad_accumulator_**`是一个指向类型为**Node**对象的`**std::weak_ptr**`指针。`**lock()**`函数的作用是创建一个`**std::shared_ptr**`来管理对象，`**try_get_grad_accumulator**`函数的返回类型是**std::shared_ptr<Node>**。<br />`**weak_ptr**`设计的目的是为配合`**shared_ptr**`而引入的一种智能指针，详见 [https://en.cppreference.com/w/cpp/memory/weak_ptr](https://en.cppreference.com/w/cpp/memory/weak_ptr) 和 [https://en.cppreference.com/w/cpp/memory/weak_ptr/lock](https://en.cppreference.com/w/cpp/memory/weak_ptr/lock) 。
<a name="hSQJd"></a>
## [7] `engine.execute(roots, grads, keep_graph, create_graph, output_edges)`
接着上边第 5 部分继续来看，最重要的`**variable_list outputs;**`的值是由函数`**engine.execute**`得到的。`**engine**`的定义如下，在文件 torch/csrc/autograd/python_engine.cpp 的第 26 行。
```cpp
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/python_engine.cpp#L26
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/python_engine.h#L12

static torch::autograd::python::PythonEngine engine;
```
`**torch::autograd::python::PythonEngine**`的定义在文件 torch/csrc/autograd/python_engine.h 中，代码如下所示。结构体`**PythonEngine**`继承自结构体`**Engine**`，而其中的方法`**execute**`也是重载的`**Engine::execute**`函数，所以要讨论的函数就变成了`**Engine::execute(roots, inputs, keep_graph, create_graph, outputs)**`。
```cpp
// python_engine.h

namespace torch { namespace autograd { namespace python {
    
    struct PythonEngine : public Engine {
        void thread_init(int device) override;
        void thread_on_exception(
            std::shared_ptr<GraphTask>& graph_task,
            const std::shared_ptr<Node>& fn,
            std::exception& e) override;
        variable_list execute(
            const edge_list& roots,
            const variable_list& inputs,
            bool keep_graph,
            bool create_graph,
            const edge_list& outputs = {}) override;
        
        variable_list execute_with_graph_task(
            std::shared_ptr<GraphTask> graph_task,
            std::shared_ptr<Node> graph_root) override;
        std::unique_ptr<AnomalyMetadata> make_anomaly_metadata() override;
    };
    
}}} // namespace torch::autograd::python

// ================================================

// python_engine.cpp

variable_list PythonEngine::execute(
    const edge_list& roots,
    const variable_list& inputs,
    bool keep_graph,
    bool create_graph,
    const edge_list& outputs) {
    try {
        return Engine::execute(roots, inputs, keep_graph, create_graph, outputs);
    } catch (python_error& e) {
        e.restore();
        throw;
    }
}
```
下边来看下文件 torch/csrc/autograd/engine.h 和 torch/csrc/autograd/engine.cpp 。
```cpp
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/engine.h
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/engine.cpp
variable_list Engine::execute(const edge_list& roots, 
                              const variable_list& inputs, 
                              bool keep_graph, 
                              bool create_graph, 
                              const edge_list& outputs)
// 调用 ↓
variable_list Engine::execute_with_graph_task(std::shared_ptr<GraphTask> graph_task,
                                              std::shared_ptr<Node> graph_root)
// 调用 ↓
void Engine::thread_main(const std::shared_ptr<GraphTask>& graph_task,
                         bool reentrant_thread)
// 调用 ↓
void Engine::evaluate_function(std::shared_ptr<GraphTask>& graph_task,
                               Node* func, 
                               InputBuffer& inputs)
// 调用 ↓ （这个函数不是 Engine 结构体中的方法）
variable_list call_function(std::shared_ptr<GraphTask>& graph_task,
                            Node* func,
                            InputBuffer& inputBuffer) {
    // ...
    auto& fn = *func;
    auto inputs = call_pre_hooks(fn, InputBuffer::variables(std::move(inputBuffer)));
    variable_list outputs = fn(std::move(inputs));
    // ...
    if(has_post_hooks){
        // NOLINTNEXTLINE(bugprone-use-after-move)
        return call_post_hooks(fn, std::move(outputs), inputs);
    }
    return outputs;
}

// =================================

static variable_list call_pre_hooks(Node& fn, 
                                    variable_list inputs) {
    for (const auto& hook : fn.pre_hooks()) {
        inputs = (*hook)(inputs);
    }
    return inputs;
}

static variable_list call_post_hooks(Node& fn, 
                                     variable_list outputs, 
                                     const variable_list& inputs) {
    for (const auto& hook : fn.post_hooks()) {
        outputs = (*hook)(outputs, inputs);
    }
    return outputs;
}

// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/function.h#L87
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/engine.h#L38
```
下边来整理下这些函数里遇到的结构体。首先是结构体**Node**，被定义在文件 torch/csrc/autograd/function.h 的第 87 行。关于**Node**，其表示一个操作，可以理解成**Autograd Graph**中的顶点**vertice**。结构体**GraphTask**被定义在文件 torch/csrc/autograd/engine.h 的第 38 行，其作用是**GraphTask holds metadata needed for a single execution of backward()**。<br />函数`**fn.post_hooks()**`和`**fn.pre_hooks()**`分别返回结构体成员变量`**post_hooks_**`和`**pre_hooks_**`，二者类型分别为`**std::vector<std::unique_ptr<FunctionPostHook>>**`和`**std::vector<std::unique_ptr<FunctionPreHook>>**`。这里又涉及到了一个结构体**struct FunctionPreHook**。关于指针`**unique_ptr**`，与`**shared_ptr**`不同，某个时刻只能有一个`**unique_ptr**`指向一个给定的对象；当`**unique_ptr**`被销毁时，它所指向的对象也被销毁，`**uniptr_ptr**`表达的是一种独占的思想。说回结构体`FunctionPreHook`和`FunctionPostHook`，这两个结构体都被定义在文件 torch/csrc/autograd/function_hook.h 中。
```cpp
// https://github.com/pytorch/pytorch/blob/master/torch/csrc/autograd/function_hook.h

#pragma once

#include <vector>
#include <torch/csrc/WindowsTorchApiMacro.h>
#include <ATen/Tensor.h>

// A hook that's called on gradients

namespace torch { namespace autograd {
    
    using Variable = at::Tensor;
    using variable_list = std::vector<Variable>;
    
    struct TORCH_API FunctionPreHook {
        virtual ~FunctionPreHook();
        virtual variable_list operator()(const variable_list& grads) = 0;
    };
    
    struct TORCH_API FunctionPostHook {
        virtual ~FunctionPostHook();
        virtual variable_list operator()(
            const variable_list& outputs /* grad_inputs */,
            const variable_list& inputs /* grad_outputs */) = 0;
    };
    
}} // namespace torch::autograd
```
