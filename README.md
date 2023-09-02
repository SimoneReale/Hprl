# Hprl


<img src="https://github.com/SimoneReale/Hprl/assets/62029302/fdb6738f-e6d2-42b6-9033-85fb885e9f6d" height="200" />
<img src="https://github.com/SimoneReale/Hprl/assets/62029302/949c3201-bdff-45f8-b9f4-d63eeeab262f" height="200" />

<br/>
<br/>
One of the most common problems faced when building an application using
OpenGL, Vulkan, or one of the other modern graphics APIs is the creation
and manipulation of textual content. They do not offer ad-hoc primitives to
solve the issue as they are oriented toward low-level management of graphics
processes and therefore they only represent the platform on which it is
possible to create complex functions like this. There are already libraries
capable of offering text-related functionalities, but often these functionalities
cannot be extracted from their context, and therefore one is forced to integrate
code that is not directly useful for the initial purposes, this not only
affects the application in terms of overhead, but in many cases it represents
a huge source of problems at build time. A deep research effort was needed
to explore beyond the boundaries of what has already been done, the challenge
accepted by HPRL is to provide a solution that is reliable, flexible,
and above all fast to implement, it is presented as a library written in C++
and contained entirely within a single header file which contains everything
necessary to start rendering text, including a default font directly embedded
in the code. In this work the domain in which HPRL operates and the main
related issues will be presented, followed by a detailed discussion on the implementation
of the library and its performance both in absolute terms and
relative to other possible options. Furthermore, some paradigmatic applications
obtained through HPRL will be presented, the aim is to demonstrate
the qualities of this work in a context as similar as possible to that of the real
world. Finally, reference will be made to possible further developments for
HPRL with particular attention to innovative techniques capable of rivaling
current standards.

