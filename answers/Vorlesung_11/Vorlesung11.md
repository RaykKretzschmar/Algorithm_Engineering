## Select one slide from the lecture, research more about the topic, and report on it.

#### Wear Leveling
 Wear Leveling ist eine wichtige Technologie in der Verwaltung von NAND-Flash-Speicher, insbesondere in SSDs, USB-Flash-Laufwerken und SD-Karten. Das Ziel ist es, die Lebensdauer dieser Speichergeräte zu verlängern, indem die Abnutzung der Speicherzellen gleichmäßig verteilt wird. Da Flash-Speicherzellen nur eine begrenzte Anzahl von Schreib- und Löschzyklen (P/E-Zyklen) überstehen können, bevor sie ausfallen, hilft Wear Leveling, die Abnutzung über alle Zellen des Geräts zu verteilen, anstatt einige Zellen häufiger als andere zu nutzen.

 Arten von Wear Leveling:

 + _Statisches Wear Leveling_: Bei dieser Methode wird nicht nur der häufig geschriebene Bereich des Speichers neu verteilt, sondern auch Daten, die selten oder nie geändert werden. Statisches Wear Leveling verschiebt auch diese "statischen" Daten gelegentlich, um sicherzustellen, dass alle Speicherzellen gleichmäßig genutzt werden. Das ist besonders wichtig für Daten, die über lange Zeit unverändert bleiben, da sie auch  zu einer ungleichmäßigen Abnutzung der Zellen beitragen können.

 + _Dynamisches Wear Leveling_: Diese Methode verteilt die Schreib- und Löschvorgänge nur über die Zellen, die aktiv genutzt werden, also über die Bereiche des Speichers, die häufig geändert werden. Dynamisches Wear Leveling ist weniger umfassend als statsches Wear Leveling, da es statische Daten nicht verschiebt, aber es ist einfacher zu implementieren und verbraucht weniger Ressourcen.

## Read the paper [Designing SSD-friendly Applications for Better Application Performance and Higher IO Efficiency](./Designing_SSD-Friendly_Applications_for_Better_Application_Performance_and_Higher_IO_Efficiency.pdf). Discuss two things you find particularly interesting.

#### SSD-friendly Desgin Changes at Other Software Layers
 _Datenstruktur-Anpassungen_:
 + Vermeidung von In-Place-Update-Optimierungen: Da SSDs Daten nicht direkt überschreiben können, führen In-Place-Updates zu zusätzlichen Lese- und Schreiboperationen, die die Performance negativ beeinflussen und die SSD schneller abnutzen. Stattdessen sollten Anwendungen so gestaltet werden, dass sie Daten an neuen Orten schreiben und die alten Daten als ungültig markieren.
 + Trennung von Hot- und Cold-Daten: Durch die physische Trennung von häufig (Hot) und selten (Cold) genutzten Daten können I/O-Effizienz gesteigert und die Abnutzung der SSD verringert werden. Dies ermöglicht es, dass Hot-Daten schneller geschrieben und gelesen werden können, ohne Cold-Daten unnötig zu bewegen oder zu aktualisieren.
 + Kompakte Datenstrukturen bevorzugen: Da SSDs in Seiteneinheiten lesen und schreiben, führt das Aktaulisieren einzelner Bytes oder kleiner Datensätze zu einem Overhead. Kompakte Datenstrukturen, die zusammenhängende Datenaktualisierungen fördern, reduzieren den Schreib-/Lesen-Overhead und verbessern die Speichereffizienz.

\
 _I/O-Handling-Anpassungen_:
 + Vermeiden von langen, intensiven Schreibvorgängen: Intensive Schreibvorgänge können die Notwendigkeit von Garbage Collection (GC) erhöhen, was die Schreibleistung beeinträchtigt. Anwendungen sollten so gestaltet werden, dass sie Daten in moderaten, gut verteilten Schreibvorgängen speichern, um die Notwendigkeit von GC zu minimieren.
 + Mischen von Schreib- und Lesevorgängen vermeiden: Gleichzeitige Schreib- und Lesevorgänge können sich gegenseitig behindern und die Gesamtleistung der SSD verringern. Wenn möglich, sollten Anwendungen Schreib- und Leseoperationen trennen, um die Effizienz zu maximieren.
 + Große, seitenalignierte I/Os bevorzugen: Große I/O-Operationen, die auf Seitengrenzen ausgerichtet sind, nutzen die SSD-Effizienz optimal aus. Unausgerichtete I/Os können zusätzliche Overhead erzeugen, da mehr Seiten gelesen und geschrieben werden müssen.
 + Volle SSD-Nutzung vermeiden: Eine vollständig genutzte SSD kann zu erhöhtem Wear-Leveling und Garbage Collection führen, was die Lebensdauer und Leistung beinträchtigt. Anwendungen sollten die SSD-Nutzung überwachen und sicherstellen, dass genügend freier Speicherplatz vorhanden ist.

\
 _Threading-Anpassungen_:
 + Mehrere Threads für kleine I/Os nutzen: SSDs profitieren von internem Parallelismus. Die Nutzung mehrerer Threads für kleine I/O-Operationen kann die Leistung verbessern, indem die internen SSD-Kanäle effizienter genutzt werden.
 + Wenige Threads für große I/Os nutzen: Für große I/O-Operationen reicht oft ein oder zwei Threads aus, um die maximale Durchsatzrate zu erreichen, da die SSD interne Parallelität bereits effizient nutzen kann. Mehr Threads können in diesem Kontext zu Ressourcenkonflikten führen und die Leistung verringern.

#### SSD-friednly Application Design
 _SSD-freundliche Designs auf der Dateisystemebene_:

 Traditionelle Dateisysteme sind oft für die Nutzung mit HDDs optimiert, bei denen sequenzieller Zugriff deutlich schneller als zufälliger Zugriff ist. SSDs hingegen eliminieren nahezu die Latenzunterschiede zwischen sequenziellem und zufälligem Zugriff, was neue Optimierungschancen bietet. Zu den SSD-spezifischen Anpassungen gehören:

 + Unterstützung für den TRIM-Befehl: Dies ermöglicht es dem Dateisystem, dem SSD-Controller mitzuteilen, welche Datenblöcke nicht mehr benötigt werden und gelöscht werden können. Dadurch kann die SSD die Wear-Leveling-Algorithmen effizienter getsalten und die Lebensdauer der SSD verlängern.
 + Log-strukturierte Datenlayouts: Im Gegensatz zu traditionellen B-Tree- oder H-Tree-Strukturen, die häufige In-Place-Aktualisierungen erfordern, können log-strukturierte Layouts die Schreibeffizienz verbessern, indem sie die Notwendigkeit des Überschreibens von Daten minimieren. Beispiele hierfür sind F2FS oder JFFS2, die speziel für die Nutzung mit Flash-Speicher entwickelt wurden.

\
 _SSD-freundliche Designs auf der Datenbankebene_:

 Datenbankmanagementsysteme (DBMS) haben jahrzehntelang Optimierungen erfahren, die auf den Eigenschaften von HDDs basieren. Die Einführung von SSDs erfordert eine Überarbeitung dieser Optimierungen:

 + Erhöhung der I/O-Konkurrenz: Während bei HDDs häufig eine 1:1-Zuordnung von Datenbankverbindung zu I/O-Thread optimal war, können SSDs von einer höheren Anzahl paralleler I/O-Operationen profitieren, um ihre interne Parallelität ausznutzen.
 + Anpassung von Datenstrukturen: Die Verschiebung von B-Bäumen zu Log-Strukturierten Bäumen oder anderen kampaktern, weniger update-intensiven Strukturen kann die Schreibleistung verbessern und die Lebensdauer der SSDs verlängern.
 + Datenlayout: Die physische Nähe von Daten, die häufig zusammen gelesen werden, kann die Effizienz von Leseoperationen verbessern. Column-oriented Layouts können in einigen Fällen vorteihafter sein als die traditionellen row-oriented Layouts.

\
 _SSD-freundliche Designänderungen auf der Ebene der Dateninfrastruktur_:

 Auf der Infrastrukturebene verändert die Einführung von SSDs traditionelle Annahmen über die Kosten und Leistung von Datenzugriffen:

 + Verminderte Notwendigkeit für verteiltes Caching: Mit SSDs kann der direkte Zugriff auf lokale Datenspeicher schneller und kosteneffizienter sein als der Zugruff auf entfernte In-Memory-Daten über das Netzwerk. Dies kann zu einer Vereinfachung der Infrastruktur führen, indem beispielsweise dedizierte Caching-Layer entfernt werden.
 + Neugestaltung von Datenverteilungsstrategien: Die traditionelle Verteilung von Daten aufgrund der Annahme, dass Fernzugriffe langsamer sind als lokale Zugriffe, muss möglicherweise überdacht werden. In einigen Fällen kann die lokale Speicherung auf SSDs günstiger und performanter sein als der Einsatz verteilter Systeme.