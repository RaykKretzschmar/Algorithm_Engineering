## How do bandwidth-bound computations differ from compute-bound computations?

__Rechenintensive Berechnungen__ haben ihren Engpass in der CPU-Leistung. Ein Beispiel hierfür ist die Schätzung von Pi durch numerische Integration, wo in einer Schleife über eine große Anzahl von Schritten iteriert wird und in jedem Schritt eine Berechnung basierend auf dem aktuellen Schritt durchgeführt wird. Diese Art der Berechnung ist stark von der Rechenleistung der CPU abhängig, da die meiste Zeit mit der Durchführung mathematischer Berechnungen verbracht wird.

__Bandbreitenintensive Berechnungen__ hingegen haben ihren Engpass im Speicherzugriff. Ein Beispiel ist die Berechnung der Summe aller Elemente in einer großen Matrix, die im Hauptspeicher gespeichert ist. Bei dieser Art von Berechnung werden große Datenmengen aus dem Speicher gelesen, und der limitierende Faktor ist die Geschwindigkeit, mit der diese Daten zwischen dem Speicher und der CPU übertragen werden können. Der Engpass liegt also nicht in der Rechenkapazität der CPU, sondern in der Bandbreite des Speichersystems.

## Explain why temporal locality and spatial locality can improve program performance.

__Zeitliche Lokalität__ bedeutet, dass ein Speicherbereich, der einmal zugegriffen wurde, mit hoher Wahrscheinlichkeit in naher Zukunft erneut zugegriffen wird. Programme mit guter zeitlicher Lokalität speichern Daten, auf die häufig zugegriffen wird, im Cache, wodurch die Notwendigkeit verringert wird, dieselben Daten immer wieder aus dem langsameren Hauptspeicher zu laden. Dies reduziert die durchschnittliche Latenzzeit für Speicherzugriffe und verbessert somit die Programmausführungsgeschwindigkeit. Ein Beispiel hierfür sind Schleifeniterationen, bei denen auf Array-Elemente zugegriffen wird; wenn das gleiche Element mehrmals in schneller Folge benötigt wird, profitiert das Programm von der zeitlichen Lokalität.

__Räumliche Lokalität__ bezieht sich auf das Prinzip, dass wenn auf eine bestimmte Speicheradresse zugegriffen wird es wahrscheinlich ist, dass in naher Zukunft auch auf die umliegenden Speicheradressen zugegriffen wird. Moderne Prozessoren und Speichersysteme sind so optimiert, dass sie ganze Datenblöcke (statt einzelner Bytes oder Wörter) in den Cache laden, wenn auf eine Adresse zugegriffen wird. Programme, die eine hohe räumliche Lokalität aufweisen, indem sie Daten sequenziell statt in zufälliger Reihenfolge verarbeiten, können so die Effizienz des Caches maximieren und die Anzahl der notwendigen Zugriffe auf den langsameren Hauptspeicher reduzieren.

## Select one slide from the lecture, research more about the topic, and report on it.

#### Streaming Stores
 Streaming Stores (Non-Temporal Stores) sind eine Technik, die in modernen Prozessoren genutzt wird, um die Effizienz von Speicheroperationen zu verbessern, besoinders in Situationen, in denen große Datenmengen verarbeitet werden müssen, die nicht für eine sofortige Wiederverwendung im Cache vorgesehen sind. Diese Technik ermöglicht es, Daten direkt in den Hauptspeicher zu schreiben, ohne dabei den Cache-Speicher zu beanspruchen oder zu verändern. 

 Die Idee dahinter ist, dass in bestimmten Anwendungsfällen das Schreiben großer Datenmengen in den Speicher nicht von unmittelbaren nachfolgenden Leseoperationen begleitet wird. Bei herkömmlichen Speicherzugriffen würde das Schreiben großer Datenmengen den Cache mit Daten füllen, die nicht sofort wieder benötigt werden, wodurch wertvoller Cache-Speicherplatz mit Daten belegt wird, die die Cache-Leistung für andere, häufiger genutzte Daten verschlchtern können.

 Der Vorteil liegt darin, dass sie es ermöglichen, den Cache für Daten freizuhalten, die eine höhere Wiederverwendungsrate haben, indem Daten, die wahrscheinlich nicht kurzfristig wieder benötigt werden, um den Cache herum und direkt in den Hauptspeicher geschrieben werden. Dies kann besonders vorteilhgaft sein, wenn große Arrays oder Datenblöcke verarbeitet werden, die nicht in den Cache passen würden oder die Cache-Leistung für andere Prozesse beeinträchtigen könnten.

 Es gibt jedoch auch Situationen, in denen die Verwendung von Streaming Stores sorgfältig abgewogen werden sollte. Falsch eingesetzt, können sie die Systemleistung negativ beeinflussen, zum Beispoel wenn die Daten doch kurz nach dem Schreiben wieder benötigt werden und dann erneut aus dem langsameren Hauptspeicher gelesen werden müssen.

 Moderne Compiler und CPUs bieten Mechanismen und Instruktionen (wie die `_mm_stream_si128` und `_mm_stream_pd` Instruktionen in x86-Architekturen), um Streaming Stores explizit in Anwendungen zu nutzen. Entwickler können diese Techniken einsetzen, um die Performance speicherintensiver Programme zu optimieren, müssen dabei jedoch die Spezifikationen und das Verhalten ihrer Zielplattformen genau verstehen und berücksichtigen.

## Read the paper [An Overview of Cache Optimization Techniques and Cache-Aware Numerical Algorithms](./3-540-36574-5_10.pdf). Discuss two things you find particularly interesting.

#### Data Access Optimizations
 Diese Optimierungen umfassen Code-Transformationen, die die Reihenfolge der Iterationen in einer Schleifenschachtelung ändern, um die zeitliche Lokalität zu verbessern. Zusätzlich können sie auch Parallelität freilegen und Schleifeniterationen vektorisierbar machen. Datenzugriffsoptimierungen beinhalten:

 + Loop Interchange: Verändert die Reihenfolge zweier benachbarter Schleifen in einer Schleifenschachtelung, was die Speicherzugriffsreihenfolge der Daten ändern und somit die Cache-Effizienz verbessern kann.

 + Loop Fusion: Kombiniert benachbarte Schleifen, die denselben Iterationsraum durchlaufen, in eine einzige Schleife, wodurch die Anzahl der Schleifenüberschriften reduziert und die Datennutzung im Cache verbessert wird.

 + Loop Blocking: Auch bekannt als Loop Tiling. Diese Technik teilt Schleifen in Blöcke auf, um die Wiederverwendung von Daten im Cache zu verbessern.

#### Data Layout Optimizations
 Diese Optimierungen verändern, wie Datenstrukturen und Variablen im Speicher angeordnet sind, um Effekte wie Cache-Konfliktmisses zu vermeiden und die räumliche Lokalität zu verbessern. Datenstrukturoptimierungen umfassen:

 + Array Padding: Fügt ungenutzte Variablen zwischen zwei Arrays ein, um "cross interference misses" zu vermeiden, die auftreten, wenn zwei Arrays im Cache zu Konflikten führen.

 + Array Merging: Wird verwendet, um die räumliche Lokalität zwischen Elementen verschiedener Arrays oder Datenstrukturen zu verbessern, indem diese so umstrukturiert werden, dass sie im Speicher kontinuierlich angeordnet sind.

 + Array Transpose: Verändert die Dimensionen innerhalb mehrdimensionaler Arrays, was ähnlich wie Loop Interchange die Cache-Nutzung optimieren kann.

 + Data Copying: Kopiert nicht-kontinuierliche Daten in einen kontinuierlichen Speicherbereich, um Selbstinterferenzen im Cache zu vermeiden und die Wiederverwendung von Cache-Daten zu verbessern.