# RCPP
Reactive library for C++

Written by
* Amar Dhingra - asd2157@columbia.edu
* Iris Zhang   - iz2140@columbia.edu
* Julie Chien  - yc2937@columbia.edu

### Thoughts
* represent streams as trees, where a stream can have an unlimited number of children, and each of the child streams knows its parent. So if we have stream A (incoming tweets) and stream B (incoming tweets that are longer than 100 chars), then B can be represented as a child of A. lazily evaluate
* define a source and its relationship with a stream more clearly
* how to integrate data propagation?
* events in a stream should be typed. But they should be able to be of many types (error types, completion types, strings, other event types). How? Maybe something like the SomeType thing from functional languages
 
### References

Wikipedia for "Observer Pattern"
* https://en.wikipedia.org/wiki/Observer_pattern

Java SE7 Observable API
* https://docs.oracle.com/javase/7/docs/api/java/util/Observable.html

Rx Docs:
* http://reactivex.io/documentation/operators.html#creating
* http://reactivex.io/documentation/observable.html

RxJava API
* http://reactivex.io/RxJava/javadoc/rx/Observable.html
* http://reactivex.io/RxJava/javadoc/index.html?rx/Observer.html

RxJS
* https://gist.github.com/staltz/868e7e9bc2a7b8c1f754
* https://github.com/Reactive-Extensions/RxJS/tree/master/doc/api/core
* https://github.com/Reactive-Extensions/RxJS/blob/master/doc/api/core/observable.md#rxobservableprototypesubscribeobserver--onnext-onerror-oncompleted

Microsoft's System.Reactive.Linq
* https://msdn.microsoft.com/en-us/library/system.reactive.linq.observable(v=vs.103).aspx 

MetaRx
* https://github.com/MetaStack-pl/MetaRx/blob/master/manual/

StackOverflow: How to implement IObservable<T>:
* http://stackoverflow.com/questions/1768974/implementing-iobservablet-from-scratch

boost::signals2
* http://www.boost.org/doc/libs/1_43_0/doc/html/signals2.html

boost::thread
* http://www.boost.org/doc/libs/1_60_0/doc/html/thread/thread_management.html#thread.thread_management.tutorial


