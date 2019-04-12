# brutils
Burak Ongay (brakulla) utility library

This a utility library consisting solutions that I came up for different projects, that I think can be reused in other projects.

`brutils` is a header only library. You can copy-paste the headers only, or use `cmake` to embed it in your project.

`src` folder includes only dummy test project. Do not use them, they are rapidly written just to prove the headers are working.

If you want to report an issue, do so by opening an issue through GitHub.

### string_utils
Static functions to ease string manipulations. Just a few of them. To be increased as long as I continue development in C++.

### timer
Very simple timer. Runs on its own thread and calls slot function given with `notify(std::function)` function on timeout. To be developed further as any development need occurs.

### queue_pc
Producer consumer queue. Only aim is to provide safe way to pass data between threads.

### br_threaded_object
`br_threaded_object` is a fully implemented class, meant to be inherited by other classes. It creates a new thread on creation, and runs itself on newly created thread with event loop. It also provides signal-slot mechanism.

It creates a thread on constructor and stops the thread on destructor (RAII). Since thread handling is done within constructor and destructor, inheriting is enough to run a class in a new thread. Its destructor is `virtual`, meaning inheriting it is safe.

In addition, it provides signal-slot mechanism similar to Qt Framework.

One can create `signal` and `slot` objects within any class, and connect them with `connect(slot)` function of `signal` objects or `connect(signal, slot)` function of `br_threaded_object`. If queued connection is required (to be used between different threads) these classes should be initialized with parent object of type `br_threaded_object`. If their parent object is set to `nullptr`, then the connection is forced to be direct.

If `signal` and `slot` objects are created with `br_threaded_object` as their parent, then there are three options for connection: auto, direct, and queued.

`Direct connection`, is a function call. When `emit()` function of a `signal` object is called, then the `slot function` of connected `slot` object is directly called as standard function call.

In `Queued connection`, the data passed as if you are doing a thread synchronization. When `emit()` function of a `signal` object is called, it is ensured that the data is sent from parent thread of `signal` object. It is also ensured that the `slot function` of connected `slot` object is called within `slot` object's parent thread.

`Auto connection` can be selected to determine the connection type automatically. The selection is based on `signal` and `slot` parent threads. If they are in the same thread, then the connection is `Direct`, otherwise `Queued`.

`br_threaded_object` has an event loop implemented with conditional variable, which causes the thread to stop fully when not in use, providing CPU efficiency. When a signal is emitted or a slot function is invoked, it is put in the event queue of respective parent threads and conditional variable is notified. So, the thread only runs when an event occurs.