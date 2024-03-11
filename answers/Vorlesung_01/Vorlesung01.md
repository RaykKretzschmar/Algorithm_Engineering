## Select one slide from the lecture, research more about the topic, and report on it.

### Welche C++ Compiler gibt es und was sind die Vor- und Nachteile dieser individuellen Compiler?

#### GNU Compiler Collection (GCC): 
Der/die GNU Compiler Collection ist ein kostenloser und quelloffener Compiler, der von der GNU Project-Gruppe entwickelt wird.

Vorteile:
- kostenlos und Open-Source
- sehr weit verbreitet
- unterstützt OpenMP

Nachteile:
- kann etwas langsamer sein als andere Compiler

#### Clang: 
Clang ist ein kostenloser und quelloffener Compiler, der von der LLVM-Projktgruppe entwickelt wird.

Vorteile:
- kostenlos und Open-Source
- unterstützt OpenMP
- schneller als GCC

Nachteile:
- nicht so weit verbreitet wie GCC

#### Microsoft Visual C++: 
Der Microsoft Visual Studio Compiler ist ein kommerzieller Compiler, der von Microsoft entwickelt wird.

Vorteile:
- integriert in Microsoft Visual Studio
- unterstützt OpenMP
- optimiert für Windows-Plattformen

Nachteile:
- kommerziell
- nicht so weit verbreitet wie GCC oder Clang


#### Weitere Compiler

- Intel C++ Compiler
- IBM XL C++ Compiler
- PathScale EKO C++ Compiler
- ARM Compiler 6

##### Zusatz:
Mittlerweile unterstützt fast jeder aktuelle C++ Compiler OpenMP.

### Compiler Phasen

**Lexikalische Analyse:** Diese Phase liest den Quellcode und zerlegt ihn in einen Strom von Tokens, die die grundlegenden Einheiten der Programmiersprache sind. Die Tokens werden dann an die nächste Phase zur weiteren Verarbeitung weitergegeben.

**Syntaxanalyse:** Diese Phase nimmt den Strom von Tokens, der von der lexikalischen Analysephase generiert wurde, und prüft, ob er der Grammatik der Programmiersprache entspricht. Das Ergebnis dieser Phase ist normalerweise ein abstrakter Syntaxbaum (AST).

**Semantische Analyse:** Diese Phase prüft, ob der Code semantisch korrekt ist, d.h. ob er dem Typsystem der Sprache und anderen semantischen Regeln entspricht.

**Generierung von Zwischencode:** Diese Phase erzeugt eine Zwischenrepräsentation des Quellcodes, die leicht in Maschinencode übersetzt werden kann.

**Optimierung:** Diese Phase wendet verschiedene Optimierungstechniken auf den Zwischencode an, um die Leistung des generierten Maschinencodes zu verbessern.

**Codegenerierung:** Diese Phase nimmt den optimierten Zwischencode und erzeugt den tatsächlichen Maschinencode, der von der Zielhardware ausgeführt werden kann.

#### 4 oder 6 Phasen?
Die sechs Phasen des Compilers können auf die vier Phasen des Compilers reduziert werden, indem die Phasen der lexikalischen Analyse und der Syntaxanalyse in die Phase des Compilers zusammengefasst werden. Die Phasen der semantischen Analyse, der Generierung von Zwischencode, der Optimierung und der Codegenerierung können in die Phase des Assemblers zusammengefasst werden. In diesem Fall würde der Compiler die Quelldatei lesen und den Objektcode direkt generieren, ohne dass eine separate Assemblierungsphase erforderlich ist. Die vier Phasen des Compilers wären dann: Preprocessing, Compilation, Assembly und Linking.

#### Was sind Vor- und Nachteile der Aufteilung in Phasen?
Die Aufteilung in Phasen hat mehrere Vorteile. Zum einen ist es einfacher, die einzelnen Schritte zu verstehen und zu überprüfen, ob sie korrekt ausgeführt wurden. Zum anderen ermöglicht es die Aufteilung, dass jeder Schritt von einem anderen Program durchgeführt werden kann, was die Modularität und Wiederverwendbarkeit des Compilers erhöht. Darüber hinaus können die verschiedenen Schritte unabhängig voneinander optimiert werden, was die Gesamtleistung des Compilers verbessert.

Es gibt auch Nachteile bei der Aufteilung in Phasen. Zum Beispiel kann es zu Problemen kommen, wenn ein Schritt fehlerhaft ist und die Ausgabe an den nächsten Schritt weitergegeben wird. In diesem Fall kann es schwierig sein, den Fehler zu finden und zu beheben. Außerdem kann die Aufteilung in Phasen dazu führen, dass der Compiler langsamer wird, da jeder Schritt zusätzliche Verarbeitungszeit benötigt.

#### Was sind andere Compiler-Arten?
Es gibt auch andere Möglichkeiten, einen Compiler zu implementieren. Einige Compiler verwenden beispielsweise eine sogenannte Just-in-Time-Kompilierung, bei der der Quellcode zur Laufzeit in Maschinencode übersetzt wird. Andere Compiler verwenden eine Interpretationstechnik, bei der der Quellcode Zeile für Zeile ausgeführt wird, ohne dass dieser in Maschinencode übersetzt wird.

Insgesamt ist die Aufteilung in Phasen jedoch eine bewährte Methode zur Implementierung eines Compilers und wird von den meisten modernen Compilern verwendet. Andere Sprachen verwenden ähnliche Techniken zur Übersetzung von Quellcode in Maschinencode.

Sources:
1. Phases of a Compiler - GeeksforGeeks. https://www.geeksforgeeks.org/phases-of-a-compiler/.
1. Phases of Compiler with Example: Compilation Process & Steps - Guru99. https://www.guru99.com/compiler-design-phases-of-compiler.html.
1. Phases of a Compiler - Coding Ninjas. https://www.codingninjas.com/studio/library/phases-of-a-compiler.
1. Phases of Compiler with Examples - GeeksforGeeks | Videos. https://www.geeksforgeeks.org/videos/phases-of-compiler-with-examples-compiler-design/.
1. Working of Compiler Phases with Example - GeeksforGeeks. https://www.geeksforgeeks.org/working-of-compiler-phases-with-example/.
1. Grouping of Phases in Compiler Design - GeeksforGeeks. https://www.geeksforgeeks.org/grouping-of-phases-in-compiler-design/.


## Read [Chapter 1](./ch1-preview.pdf) from Computer Systems: A Programmer's Perspective - Discuss two things you find particularly interesting
http://csapp.cs.cmu.edu/2e/ch1-preview.pdf

### Caches in Computersystemen

#### Grundlagen und Rolle von Caches
- **Definition**: Caches sind spezielle Speichereinheiten, die Daten temporär speichern, um den Zugriff auf häufig genutzte Informationen zu beschleunigen.
- **Zweck**: Sie dienen dazu, die Lücke zwischen der hohen Verabreitungsgeschwindigkeit des Prozessors und der langsameren Geschwindigkeit des Hauptspeichers zu überbrücken.

#### Organisationsstruktur
- **Ebenen**: Caches sind in Ebenen organisiert (L1, L2, L3), wobei jede Ebene ihre eigene Größe und Geschwindigkeit hat.
- **Nähe zum Prozessor**: L1-Cache ist der schnellste und befindet sich am nächsten am Prozessor.

#### Technologie und Leistung
- **Speichertechnologie**: Caches nutzen in der Regel SRAM (Static Random-Access Memory), das schneller als der DRAM des Hauptspeichers ist.
- **Leistungssteigerung**: Durch die Nutzung von Caches können Datenzugriffe erheblich beschleunigt werden, was zu einer Gesamtsteigerung der Systemleistung führt.

#### Prinzip der Lokalität
- **Arten der Lokalität**: Lokalität bezieht sich auf das Muster, nach dem Programme auf Daten und Instruktionen zugreifen. Sie kann in räumliche und zeitliche Lokalität unterteilt werden.

#### Auswirkungen auf die Softwareentwicklung
- **Programmoptimierung**: Ein Verständnis der Cache-Architektur und Funktionsweise ermöglicht es Entwicklern, ihre Software so zu optimieren, dass sie effizienter auf Daten zugreift und die Cache-Nutzung maximiert.

#### Herausforderungen und Überlegungen
- **Cache-Kohärenz**: Bei Systemen mit mehreren Prozessoren oder Caches ist die Aufrechterhaltung der Datenkonsistenz zwischen verschiedenen Caches eine wichtige Herausforderung.

### Verständnis von Kompilationssystemen

#### Wichtigkeit für die Programmoptimierung
- **Grundverständnis erforderlich**: Für effizientes Programmieren ist ein Basiswissen über maschinennahen Code und dessen Kompilierung wichtig.
- **Beispiele für Entscheidungen**: Auswahl zwischen `switch` und `if-else`, Verständnis der Overhead-Kosten von Funktionsaufrufen, Schleifenoptimierung, Effizienz von Pointern gegenüber Array-Indizes.
- **Leistungssteigerung durch Code-Transformationen**: Einfache Änderungen im C-Code können die Kompilierungseffizienz steigern.

#### Verständnis von Linker-Fehlern
- **Komplexe Fehlerquellen**: Linker-Fehler, besonders in großen Softwareprojekten, können schwer zu verstehen sein.
- **Typische Fragen**: Umgang mit nicht aufgelösten Referenzen, Unterschied zwischen statischen und globalen Variablen, Konflikte bei gleichnamigen globalen Variablen.

#### Vermeidung von Sicherheitslücken
- **Buffer Overflow-Problematik**: Ein tiefes Verständnis der Daten- und Kontrollinformationsspeicherung auf dem Program Stack ist für sichere Programmierung entscheidend.
- **Sicherheitsmaßnahmen**: Methoden zur Reduzierung von Sicherheitsbedrohungen durch den Programmierer, Compiler und das Betriebssystem.