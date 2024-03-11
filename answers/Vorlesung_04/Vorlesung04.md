## Benchmark min_max_quicksort against std::sort and __gnu_parallel::sort.

Operating System: Windows 11 Home, Version 10.0.22631 Build 22631 \
RAM: 32 GB DDR5 5600 MHz \
CPU: 12th Gen Intel(R) Core(TM) i9-12900K, 3200 MHz, 16 cores, 24 logical processors \
Compiler: g++ 13.2.0 (Rev3, Built by MSYS2 project) \
Compiler Flags: -Ofast -std=c++17 -fopenmp

## Read [What every systems programmer should know about concurrency](./concurrency-primer.pdf). Discuss two things you find particularly interesting.

#### Atomic Fusion

Atomic Fusion bezieht sich auf die vielfältigen Optimierungsmöglichkeiten, die moderne Compiler bei der Ausführung von atomaren Operationen nutzen können. Ein besonders interessanter Aspekt dabei ist, dass Compiler utner gewissen Umständen in der Lage sind, atomare Operationen zu optimieren, indem sie beispielsweise mehrere Operationen zusammenführen („fusing“). Dies kann allerdings zu unerwarteten Verhaltensweisen führen, vor allem wenn eine strikte Ausführungsreihenfolge für die Korrektheit des Programms essentiell ist. Um solche Probleme zu vermeiden, müssen Entwickler gelegentlich auf spezifische Techniken zurückgreifen, wie dem Einsatz von volatile-Zugriffen oder speziellen Compiler-Befehlen, die verhindern, dass der Compiler Optimierungen durchführt, welche die erwartete Ausführungsreihenfolge ändern könnten. Dies unterstreicht eindrucksvoll die Komplexität, die mit der Entwicklung von korrektem und effizientem nebenläufigem Code verbunden ist, und betont die Notwendigkeit eines tiefen Verständnisses der zugrunde liegenden Hardware und des Verhaltens der Compiler.

#### If concurrency is the question, volatile is not the answer

Dieses Thema klärt ein weit verbreitetes Missverständnis auf, nämlich dass das Schlüsselwort volatile in Sprachen wie C oder C++ für die Synchronisation zwischen Threads genutzt werden kann. Tatsächlich bietet volatile keine Garantien hinsichtlich der Atomarität oder der Sichtbarkeit von Änderungen zwischen Threads. Es verhindert lediglich, dass der Compiler Zugriffe auf volatile Variablen optimiert, was bedeutet, dass jede Lese- oder Schreiboperation tatsächlich ausgeführt wird. Jedoch, ohne spezielle Speicherbarrieren oder Atomaritätsgarantien, können volatile-Variablen nicht sicher für die Synchronisation in nebenläufigen Programmen verwendet werden. Dies unterstreicht die Bedeutung, geeignete Synchronisationsmechanismen wie Mutex, Semaphore oder atomare Opertionen zu verwenden, um konsistente Zustände zwischen Threads zu gewährleisten und zeigt deutlich, wie essentiell ein gründliches Verständnis dieser Konzepte für die Entwicklung nebenläufiger Software ist.
