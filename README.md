# brutils
Burak Ongay (brakulla) utility library

This a utility library consisting solutions that I come up for different projects, and can be used just more than one place.

### br_threaded_object
`br_threaded_object` is a fully implemented class, meant to be inherited by other classes. It creates a new thread on creation, and runs itself on newly created thread. It also provides signal-slot mechanism, as explained further below.

This class is inspired from how `QObject` of Qt Framework work, to provide signal-slot mechanism which is also inspired from Qt Framework. 

One can create `signal` and `slot` objects within any class, and connect them. However, if their parent object is not set to `br_threaded_object` instances, then the connection is forced to be direct.

If `signal` and `slot` objects are created with `br_threaded_object` as their parent, then there are three options for connection: auto, direct, and queued.

`Direct connection`, is a function call. When `emit()` function of a `signal` object is called, then the `slot function` of connected `slot` object is a direct function call.

In `Queued connection`, the data passed as if you are doing a thread synchronization. When `emit()` function of a `signal` object is called, it is ensured that the data is sent from parent thread of `signal` object. It is also ensured that the `slot function` of connected `slot` object is called within `slot` object's parent thread.

`Auto connection` can be selected to determine the connection type automatically. The selection is based on `signal` and `slot` parent threads. If they are in the same thread, then the connection is `Direct`, otherwise `Queued`.
