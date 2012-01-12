<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="es_ES">
<context>
    <name>QULyricTask</name>
    <message>
        <location filename="QULyricTask.cpp" line="16"/>
        <source>Set first timestamp to zero</source>
        <translation>Fijar el primer marcador de tiempo en cero</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="20"/>
        <source>Fix overlapping notes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="21"/>
        <source>Shortens adjacent notes to remove overlapping.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="25"/>
        <source>Fix line break timings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="26"/>
        <source>Moves line break timing to the middle of one line&apos;s end and the next line&apos;s beginning.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="30"/>
        <source>Fix spaces</source>
        <translation>Corregir espacios</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="31"/>
        <source>Moves spaces from the end of a syllable to the beginning of the next one. Trim the whole song line.</source>
        <translation>Mueve los espacios del final de las sílabas hacia el principio de las siguientes. Ajusta toda la línea.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="35"/>
        <source>Increase low BPM values</source>
        <translation>Aumentar valores bajos de BPM</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="36"/>
        <source>Doubles BPM value and all note timings while it is below a certain threshold.</source>
        <translation>Duplica el valor del BPM y el tiempo de todas las notas mientras esté bajo cierto rango.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="40"/>
        <source>Set medley and preview tags if not present</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="41"/>
        <source>Tries to determine a suitable medley section and sets medley tags accordingly, if successful.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="45"/>
        <source>Capitalize first word of each song line</source>
        <translation>Poner en mayúscula la primera palabra de cada línea</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="49"/>
        <source>Normalize pitch values</source>
        <translation>Normalizar tonos</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="50"/>
        <source>Shifts all note pitches octavewise towards a zero mean.</source>
        <translation>Cambia el tono de todas las notas en octavas hasta que tengan media cero.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="54"/>
        <source>Remove empty syllables</source>
        <translation>Eliminar sílabas vacías</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="55"/>
        <source>Example:&lt;br&gt;&lt;br&gt;: 230 6 9 be&lt;br&gt;&lt;b&gt;: 236 5 10&lt;/b&gt;&lt;br&gt;: 241 7 3 not.&lt;br&gt;&lt;br&gt;Remove the bold one.</source>
        <translation>Ejemplo:&lt;br&gt;&lt;br&gt;: 230 6 9 no&lt;br&gt;&lt;b&gt;: 236 5 10&lt;/b&gt;&lt;br&gt;: 241 7 3 ser.&lt;br&gt;&lt;br&gt;Elimina la que está en negrita.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="58"/>
        <source>Convert syllable placeholders from &quot;-&quot; to &quot;~&quot;</source>
        <translation>Convertir marcadores de posición de las sílabas de &quot;-&quot; a &quot;~&quot;</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="63"/>
        <source>Convert syllable placeholders from &quot;~&quot; to &quot;-&quot;</source>
        <translation>Convertir marcadores de posición de las sílabas de &quot;~&quot; a &quot;-&quot;</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="68"/>
        <source>Convert relative to absolute timestamps</source>
        <translation>Convertir marcadores de tiempo relativos a absolutos</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="73"/>
        <source>Convert absolute to relative timestamps</source>
        <translation>Convertir marcadores de tiempo absolutos a relativos</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="127"/>
        <source>Start:</source>
        <translation>Inicio:</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="443"/>
        <source>BPM changed from %1 to %2 for &quot;%3 - %4&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="461"/>
        <source>Skipping medley search: medley tags already set for &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="466"/>
        <source>Skipping medley search: medley tags not supported for duet &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="471"/>
        <source>Skipping medley search: relative format not supported for &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="476"/>
        <source>Skipping medley search: search disabled by CALCMEDLEY tag for song &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="518"/>
        <source>No medley candiates found for song &quot;%1 - %2&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="569"/>
        <source>Medley with a duration of %1 seconds set for &quot;%2 - %3&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="577"/>
        <source>No suitable medley section found for &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>if BPM &lt;</source>
        <oldsource>if below:</oldsource>
        <translation type="obsolete">si BPM &lt;</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="145"/>
        <location filename="QULyricTask.cpp" line="298"/>
        <location filename="QULyricTask.cpp" line="398"/>
        <location filename="QULyricTask.cpp" line="456"/>
        <source>Invalid lyrics in file &quot;%1&quot;</source>
        <translation>Letra inválida en el archivo &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="346"/>
        <source>Line break timings adjusted for &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="225"/>
        <location filename="QULyricTask.cpp" line="358"/>
        <location filename="QULyricTask.cpp" line="588"/>
        <location filename="QULyricTask.cpp" line="609"/>
        <location filename="QULyricTask.cpp" line="656"/>
        <location filename="QULyricTask.cpp" line="680"/>
        <location filename="QULyricTask.cpp" line="706"/>
        <location filename="QULyricTask.cpp" line="754"/>
        <source>Invalid lyrics: %1 - %2</source>
        <translation>Letra inválida: %1 - %2</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="130"/>
        <source>if BPM less than:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="133"/>
        <source>Minimum length:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="133"/>
        <source>seconds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="134"/>
        <source>Overwrite existing values</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="171"/>
        <source>#GAP changed from %1 to %2 for &quot;%3 - %4&quot;.</source>
        <translation>Cambió #GAP de %1 a %2 para &quot;%3 - %4&quot;.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="214"/>
        <source>Timestamps were changed successfully for &quot;%1 - %2&quot;.</source>
        <translation>Los marcadores de tiempo cambiaron exitosamente para &quot;%1 - %2&quot;.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="274"/>
        <source>Unable to fix out of sequence notes at beats %1 to %2 for &quot;%3 - %4&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="281"/>
        <source>Adjusted medley end beat from %1 to %2 for &quot;%3 - %4&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="290"/>
        <source>%1 of %2 overlapping notes were fixed successfully for &quot;%3 - %4&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="390"/>
        <source>Spaces were fixed successfully for &quot;%1 - %2&quot;.</source>
        <translation>Los espacios fueron corregidos exitosamente para &quot;%1 - %2&quot;.</translation>
    </message>
    <message>
        <source>#BPM changed from %1 to %2 for &quot;%3 - %4&quot;.</source>
        <translation type="obsolete">El #BPM cambió de %1 a %2 para &quot;%3 - %4&quot;.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="599"/>
        <source>Line capitalization fixed for &quot;%1 - %2&quot;.</source>
        <translation>Mayúsculas de línea arregladas para &quot;%1 - %2&quot;.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="643"/>
        <source>Mean note pitch changed from %1 to %2 for &quot;%3 - %4&quot;.</source>
        <translation>Media de tonos cambió de %1 a %2 para &quot;%3 - %4&quot;.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="675"/>
        <source>Empty syllables were removed successfully for &quot;%1 - %2&quot;.</source>
        <translation>Se eliminaron sílabas vacías exitosamente para &quot;%1 - %2&quot;.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="694"/>
        <source>Syllable placeholders were converted successfully from &apos;%3&apos; to &apos;%4&apos; for &quot;%1 - %2&quot;.</source>
        <translation>Marcadores de posición de sílabas fueron convertidos exitosamente de &apos;%3&apos; a &apos;%4&apos; para &quot;%1 - %2&quot;.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="744"/>
        <source>Relative timestamps converted successfully to absolute timestamps for &quot;%1 - %2&quot;.</source>
        <translation>Marcadores de tiempo fueron convertidos exitosamente de relativos a absolutos para &quot;%1 - %2&quot;.</translation>
    </message>
    <message>
        <location filename="QULyricTask.cpp" line="785"/>
        <source>Absolute timestamps converted successfully to relative timestamps for &quot;%1 - %2&quot;.</source>
        <translation>Marcadores de tiempo fueron convertidos exitosamente de absolutos a relativos para &quot;%1 - %2&quot;.</translation>
    </message>
</context>
<context>
    <name>QULyricTaskFactory</name>
    <message>
        <location filename="QULyricTaskFactory.cpp" line="11"/>
        <source>Lyric Tasks</source>
        <translation>Tareas de la Letra</translation>
    </message>
    <message>
        <location filename="QULyricTaskFactory.cpp" line="15"/>
        <source>Modify a song&apos;s lyrics.</source>
        <translation>Cambia la letra de una canción.</translation>
    </message>
</context>
<context>
    <name>QUMultilingualTaskFactory</name>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="25"/>
        <source>Unnamed plugin</source>
        <translation>Plugin desconocido</translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="27"/>
        <source>Unknown person</source>
        <translation>Persona desconocida</translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="28"/>
        <source>No description given.</source>
        <translation>Sin descripción.</translation>
    </message>
</context>
<context>
    <name>QUSimpleTaskFactory</name>
    <message>
        <location filename="../shared/QUSimpleTaskFactory.h" line="16"/>
        <source>Simple Task</source>
        <translation>Tarea simple</translation>
    </message>
</context>
<context>
    <name>QUSmartSetting</name>
    <message>
        <location filename="../shared/QUSmartSetting.cpp" line="30"/>
        <source>Reset to default value.</source>
        <translation>Reestablecer a valor por defecto.</translation>
    </message>
</context>
</TS>
