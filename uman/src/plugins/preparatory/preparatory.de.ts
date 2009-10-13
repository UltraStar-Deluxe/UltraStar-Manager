<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="de_DE">
<context>
    <name>QUMultilingualTaskFactory</name>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="25"/>
        <source>Unnamed plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="27"/>
        <source>Unknown person</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="28"/>
        <source>No description given.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QUPreparatoryTask</name>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="20"/>
        <source>Assign missing files automatically</source>
        <translation>Fehlende Dateien automatisch zuweisen</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="29"/>
        <source>Remove unsupported tags</source>
        <translation>Nicht unterstützte Tags entfernen</translation>
    </message>
    <message>
        <source>&lt;b&gt;#MP3&lt;/b&gt; ... first audio file found.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... first picture file, including pattern: &lt;i&gt;[CO]|cove?r?&lt;/i&gt;.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... first picture file, including pattern: &lt;i&gt;[BG]|back&lt;/i&gt;.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... first video file found.</source>
        <translation type="obsolete">&lt;b&gt;#MP3&lt;/b&gt; ... erste Audiodatei.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... erstes Bild mit dem Muster: &lt;i&gt;[CO]|cove?r?&lt;/i&gt;.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... erstes Bild mit dem Muster: &lt;i&gt;[BG]|back&lt;/i&gt;.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... erstes Video.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="21"/>
        <source>&lt;b&gt;#MP3&lt;/b&gt; ... first audio file found.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... first picture file, following a pattern.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... first picture file, following a pattern.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... first video file found.&lt;br&gt;&lt;br&gt;Does not reset tags that have already been set.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="33"/>
        <source>Fix audio length via #END tag</source>
        <translation>Audio-Länge mittels #END-Tag reparieren</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="34"/>
        <source>Sets &lt;b&gt;#END&lt;/b&gt; to the length of the song if audio file present and longer than song. &lt;b&gt;Appends a little time buffer.&lt;/b&gt;</source>
        <translation>Setzt &lt;b&gt;#END&lt;/b&gt; auf die Länge des Songs, sofern eine Audio-Datei existiert, die länger als der Song ist. &lt;b&gt;Beinhaltet einen kleinen Zeitpuffer.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="38"/>
        <source>Round #GAP to nearest integer.</source>
        <translation>Runde #GAP auf die nächste Ganzzahl.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="42"/>
        <source>Fix capitalization of #TITLE tag</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="43"/>
        <source>Applies some rules of capitalization to form a consistent appearance for all song titles.&lt;br&gt;&lt;br&gt;&lt;b&gt;Useful for english songs.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="47"/>
        <source>Capitalize #ARTIST tag</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="48"/>
        <source>Capitalize each word of the artist, e.g. &quot;michael jackson&quot; -&gt; &quot;Michael Jackson&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="104"/>
        <source>Capitalize English songs only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="105"/>
        <source>Capitalize each word</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="106"/>
        <location filename="QUPreparatoryTask.cpp" line="109"/>
        <source>Capitalize first word only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="148"/>
        <source>Capitalization fix not applicable due to non-english song: &quot;%1 - %2&quot;. Try to configure the task.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="156"/>
        <source>Capitalization fix not applicable due to empty title: &quot;%1&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="221"/>
        <source>Capitalization of #TITLE fixed for &quot;%1 - %2&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="234"/>
        <source>Capitalization fix not applicable due to empty artist: &quot;%1&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="249"/>
        <source>Capitalization of #ARTIST fixed for &quot;%1 - %2&quot;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QUPreparatoryTaskFactory</name>
    <message>
        <location filename="QUPreparatoryTaskFactory.cpp" line="11"/>
        <source>Preparatory Tasks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTaskFactory.cpp" line="15"/>
        <source>Fix general issues and prepare song for other tasks.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QUSimpleTaskFactory</name>
    <message>
        <location filename="../shared/QUSimpleTaskFactory.h" line="16"/>
        <source>Simple Task</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QUSmartSetting</name>
    <message>
        <location filename="../shared/QUSmartSetting.cpp" line="30"/>
        <source>Reset to default value.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
