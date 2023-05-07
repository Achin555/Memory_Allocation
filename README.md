# Memory_Allocation

<b>\documentclass{article}</b>

<b>\begin{document}</b>

<b>\title
 
<b>{Memory Allocator}</b>

<b>\author</b>

<b>{Achintya Misra,Sidharth Thakur,Shaurya Nagpal}</b>

<b>\maketitle</b>

\section
{Introduction}
This is a simple implementation of a memory allocator using the buddy allocation algorithm. The allocator is designed to manage a fixed-size block of memory, which is divided into smaller blocks of a fixed size. It keeps track of which blocks are in use and which are free, and allocates blocks to satisfy memory requests. The allocator is written in C++ and uses the Windows API for memory allocation.

\section{Usage}
To use the allocator, include the "memory_allocator.h" header file in your project and create an instance of the allocator:

\begin{verbatim}
#include "memory_allocator.h"

MemoryAllocator allocator(size);
\end{verbatim}

where \texttt{size} is the total size of the memory block to manage.

To allocate memory, use the \texttt{allocate} function:

\begin{verbatim}
void* ptr = allocator.allocate(size);
if (ptr == nullptr) {
// allocation failed
}
\end{verbatim}

where \texttt{size} is the size of the memory block to allocate.

To deallocate memory, use the \texttt{deallocate} function:

\begin{verbatim}
allocator.deallocate(ptr);
\end{verbatim}

where \texttt{ptr} is the pointer to the memory block to deallocate.

\section{Implementation}
The implementation of the allocator consists of the following steps:

\begin{enumerate}
\item Allocate a large block of memory using the \texttt{VirtualAlloc} function from the Windows API.
\item Divide the memory block into smaller blocks of a fixed size using a power-of-two strategy.
\item Keep track of which blocks are in use and which are free using an array of integers, where each integer represents a group of blocks of a particular size.
\item Implement the \texttt{allocate} and \texttt{deallocate} functions using the buddy allocation algorithm, which searches for the smallest available block that is big enough to satisfy the request and splits or merges blocks as necessary.
\end{enumerate}

\section{Conclusion}
This memory allocator implementation provides a simple example of how the buddy allocation algorithm can be used to manage a fixed-size block of memory. While this implementation is limited to the Windows platform and has some limitations in terms of efficiency and fragmentation, it can serve as a starting point for more advanced memory allocation techniques.
