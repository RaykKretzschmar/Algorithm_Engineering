## Read [Chapter 3](./Chapter3.pdf) titled Programming Multi-core and Shared Memory Multiprocessors Using OpenMP (pp. 47–78) in Introduction to Parallel Computing (Trobec et al., 2018).

### When run with one thread per logical core, threads from Listing 3.1 display their numbers randomly, while those from Listing 3.2 show them in the same order. Explain why.

#### **Zufällige Reihenfolge in Listing 3.1**: 
  Die Zufälligkeit in der Anzeige der Threadnummern in Listing 3.1 ergibt sich aus der einfachen Natur der Aufgabe (Ausgabe der Threadnummern) und dem Fehlen einer expliziten Synchronisation oder Ordnung. Das Betriebssystem und der OpenMP-Scheduler bestimmen die Ausführungsreihenfolge, was zu Zufälligkeit führt.

#### **Geordneter Abschluss in Listing 3.2**: 
  Im Gegensatz dazu beinhaltet Listing 3.2 eine zeitaufwändigere Aufgabe. Die Reihenfolge des Abschlusses der Threads könnte durch die Art der Aufgabe und deren Ausführungszeit beeinflusst werden. Wenn die Rechenlast signifikant ist, könnte sie die Zufälligkeit der Planung überschaatten, was zu einem vorhersehbareren Abschluss der Threads führt, besonders gegen Ende der Programmausführung.

### What do you think, does the code snippet in Listing 3.10 use the cores efficiently, why yes, why no. Consider also the variable size in your argumentation.

#### **Ausnutzung von Mehrkernprozessoren**:
  Der Code nutzt die `#pragma omp parallel for collapse (2)`-Direktive, um die Arbeit über mehrere Kerne zu verteilen. Dies ist besonders effektiv, da jede Iteration der Schleifen unabhängig ist, was eine Grundvoraussetzung für effektive Parallelisierung ist.

#### **Größe der Aufgabe (`size`)**:
  Die Variable `size` bestimmt die Größe des Spielfelds. Bei kleinen Werten könnte der Parallelisierungs-Overhead die Leistung mindern, während bei großen Werten die Parallelisierung wahrscheinlich effizient ist, da mehr Berechnungen über die Kerne verteilt werden.

#### **Lastverteilung**:
  Die `collapse (2)`-Direktive sorgt für eine gleichmäßigere Verteilung der Arbeit auf die Threads, was bei großen Werten von `size` eine gleichmäßige Auslastung der Kerne gewährleistet.

#### **Synchronisierung und Kommunikation**:
  Eine implizite Synchronisierung nach der Beendigung aller Iterationen ist notwendig, um die Korrektheit der parallelen Verarbeitung zu gewährleisten. Der Synchronisierungsaufwand könnte bei sehr großen Spielfeldern signifikant sein.

#### Schlussfolgerung:
Die Effizienz der Parallelisierung in Listing 3.10 hängt stark von der Größe des Spielfelds (`size`) und der Anzahl der verfügbaren Kerne ab. Sie ist wahrscheinlich effizient für große Spielfelder und Systeme mit vielen Kernen, während bei kleineren Spielfeldern oder weniger Kernen der Aufwand die Leistungsgewinne überwiegen könnte.

### From Lecture 1, take your parallelized version of pi_monte_carlo.cpp and modify it based on the insights from Example 3.5 on computing π using random shooting, particularly the random number generation method. Report the changes you do and their impact on performance.

#### Änderungen am Code
Die Berechnung der Zufallszahlen wurde geändert. Im Code von [pi_estimate_parallel](../Vorlesung_01/pi_estimate_parallel.cpp) erfolgt die Berechnung von x und y durch:

    default_random_engine re{seed};
    uniform_real_distribution<double> zero_to_one{0.0, 1.0};
// ...
    
    auto x = zero_to_one(re); // generate random number between 0.0 and 1.0
    auto y = zero_to_one(re); // generate random number between 0.0 and 1.0

Im Code von [pi_estimate_parallel_modified](../Vorlesung_03/pi_estimate_parallel_modified.cpp) wurde diese Berechnung durch die rnd-Methode aus Example 3.5 ersetzt:

    double rnd(unsigned int *seed)
    {
        *seed = (1140671485 * (*seed) + 12820163) % (1 << 24);
        return ((double)(*seed)) / (1 << 24);
    }
// ...

    auto x = rnd(&seed); // generate random number between 0.0 and 1.0
    auto y = rnd(&seed); // generate random number between 0.0 and 1.0

Dies führt zu einer deutlichen Perfomance-Steigerung:

| Durchlauf | pi_estimate_parallel (s) | pi_estimate_parallel_modified (s) |
|-----------|---------------------------|-----------------------------------|
| 1         | 1.6871                    | 0.499267                          |
| 2         | 1.72913                   | 0.442068                          |
| 3         | 1.75736                   | 0.46211                           |

Die Laufzeit vom nicht modifizierten pi_estimate_parallel ist drei mal so hoch wie die von pi_estimate_parallel_modified.

#### Grund für die Laufzeitverbesserung

`default_random_engine` und `uniform_real_distribution<double>` sind Standardbibliothekmethoden, für die Generierung von Zufallszahlen. Sie sind robuster und generieren qualitativ hochwertigere Zufallszahlen. Allerdings werden komplexere Algorithmen benutzt, die zusätzlichen Overhead mitbringen, da jeder Thread die Zufallszahl selbst berechnen muss. Das verlangsamt die Berechnung von `pi`.

Die Methode rnd aus Example 3.5 hingegen ist einfacher und damit schneller, aber potenziell weniger zufällig oder gleichmäßig vertielt als die Standardmethoden. Sie eignet sich gut für Anwendungen, bei denen Geschwindigkeit wichtiger ist als die Qualität der Zufallszahlen.