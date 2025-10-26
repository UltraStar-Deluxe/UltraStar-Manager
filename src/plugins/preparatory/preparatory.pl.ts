<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="pl_PL">
<context>
    <name>QUMultilingualTaskFactory</name>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="25"/>
        <source>Unnamed plugin</source>
        <translation>Nienazwana wtyczka</translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="27"/>
        <source>Unknown person</source>
        <translation>Nieznana osoba</translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="28"/>
        <source>No description given.</source>
        <translation>Nie podano opisu.</translation>
    </message>
</context>
<context>
    <name>QUPreparatoryTask</name>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="20"/>
        <source>Assign missing files automatically</source>
        <translation>Przypisz automatycznie brakujące pliki</translation>
    </message>
    <message>
        <source>&lt;b&gt;#MP3&lt;/b&gt; ... first audio file found.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... first picture file, including pattern: &lt;i&gt;[CO]|cove?r?&lt;/i&gt;.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... first picture file, including pattern: &lt;i&gt;[BG]|back&lt;/i&gt;.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... first video file found.</source>
        <translation type="obsolete">&lt;b&gt;#MP3&lt;/b&gt; ... pierwszy znaleziony plik z muzyką.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... pierwszy plik graficzny, który zawiera w nazwie: &lt;i&gt;[CO]&lt;/i&gt;.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... pierwszy plik graficzny, który zawiera w nazwie: &lt;i&gt;[BG]&lt;/i&gt;.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ...pierwszy plik z teledyskiem.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="21"/>
        <source>&lt;b&gt;#MP3&lt;/b&gt; ... first audio file found.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... first picture file, following a pattern.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... first picture file, following a pattern.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... first video file found.&lt;br&gt;&lt;br&gt;Does not reset tags that have already been set.</source>
        <translation>&lt;b&gt;#MP3&lt;/b&gt; ... pierwszy znaleziony plik z muzyką&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... pierwszy plik graficzny&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... pierwszy plik graficzny.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... pierwszy plik teledysku znaleziony.&lt;br&gt;&lt;br&gt;Nie resetuje eykiet, które zostały już ustawione.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="29"/>
        <source>Fix or remove unsupported tags</source>
        <oldsource>Remove unsupported tags</oldsource>
        <translation type="unfinished">Usuwanie nierozpoznanych etykiet</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="30"/>
        <source>Replaces the incorrect &lt;b&gt;#AUTHOR&lt;/b&gt; tag by the correct &lt;b&gt;#CREATOR&lt;/b&gt; tag and deletes all other unsupported tags.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="34"/>
        <source>Fix audio length via #END tag</source>
        <translation>Popraw długość utworu w oparciu o etykietę #END</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="35"/>
        <source>Sets &lt;b&gt;#END&lt;/b&gt; to the length of the song if audio file present and longer than song. &lt;b&gt;Appends a little time buffer.&lt;/b&gt;</source>
        <translation>Ustawia &lt;b&gt;#END&lt;/b&gt; z wpisem długości utworu, przydatne gdy plik z muzyką jest dłuższy od utworu. &lt;b&gt;Dodaje nieco czasu.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="39"/>
        <source>Round #GAP to nearest integer</source>
        <oldsource>Round #GAP to nearest integer.</oldsource>
        <translation>Przybliż #GAP do najbliższej liczby po przecinku</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="43"/>
        <source>Capitalize #TITLE tag</source>
        <oldsource>Fix capitalization of #TITLE tag</oldsource>
        <translation>&quot;Wielką Literą&quot; tytuł</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="44"/>
        <source>Applies some rules of capitalization to form a consistent appearance for all song titles.&lt;br&gt;&lt;br&gt;&lt;b&gt;Useful for english songs.&lt;/b&gt;</source>
        <translation>Funkcja &quot;Wielką Literą&quot; zmienia pierwsze małe litery na wielkie tworząc spójny wygląd tytułów piosenek.&lt;br&gt;&lt;br&gt;&lt;b&gt;Uzywaj do angielskich piosenek.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="48"/>
        <source>Capitalize #ARTIST tag</source>
        <translation>&quot;Wielką Literą&quot; wykonawca</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="49"/>
        <source>Capitalize each word of the artist, e.g. &quot;michael jackson&quot; -&gt; &quot;Michael Jackson&quot;.</source>
        <translation>&quot;Wielką Literą&quot; nazwa wykonawcy, np. &quot;sylwia grzeszczak&quot; -&gt; &quot;Sylwia Grzeszczak&quot;.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="53"/>
        <source>Set #EDITION tag if it is empty</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="54"/>
        <source>Sets &lt;b&gt;#EDITION&lt;/b&gt; to a defined value if it is empty or not present.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="58"/>
        <source>Set #GENRE tag if it is empty</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="59"/>
        <source>Sets &lt;b&gt;#GENRE&lt;/b&gt; to a defined value if it is empty or not present.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="63"/>
        <source>Fix #LANGUAGE tag to contain English language names</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="64"/>
        <source>Sets &lt;b&gt;#LANGUAGE&lt;/b&gt; to contain English language names (e.g. Deutsch -&gt; German).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="68"/>
        <source>Fix apostrophes in song artist and title</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="69"/>
        <source>Replaces wrongfully used apostrophe symbols `, ´, &apos; by the correct apostrophe ’</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="73"/>
        <source>Add missing ’P1’ and ’P2’ in duets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="74"/>
        <source>Detects if a song is a duet that is missing ’P1’ and ’P2’ tags and adds them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="134"/>
        <location filename="QUPreparatoryTask.cpp" line="135"/>
        <source>Pattern:</source>
        <translation>Wzór:</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="134"/>
        <source>(cover)</source>
        <translation>(okładka)</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="135"/>
        <source>(background)</source>
        <translation>(tło)</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="142"/>
        <source>Buffer:</source>
        <translation>Bufor:</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="142"/>
        <source>seconds</source>
        <translation>sekundy</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="145"/>
        <source>Capitalize English songs only</source>
        <translation>&quot;Wielką Literą&quot; tylko angielskie piosenki</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="146"/>
        <source>Capitalize each word</source>
        <translation>&quot;Wielką Literą&quot; każde słowo</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="147"/>
        <location filename="QUPreparatoryTask.cpp" line="150"/>
        <source>Capitalize first word only</source>
        <translation>&quot;Wielką Literą&quot; tylko pierwsze słowo</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="153"/>
        <source>Edition:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="156"/>
        <source>Genre:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="195"/>
        <source>Capitalization fix not applicable due to non-english song: &quot;%1 - %2&quot;. Try to configure the task.</source>
        <translation>&quot;Wielką Literą&quot; nie może być zastosowane do nieangielskich piosenek: &quot;%1 - %2&quot;. Spróbuj zmienić zadanie.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="203"/>
        <source>Capitalization fix not applicable due to empty title: &quot;%1&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="268"/>
        <source>Capitalization of #TITLE fixed for &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="281"/>
        <source>Capitalization fix not applicable due to empty artist: &quot;%1&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="296"/>
        <source>Capitalization of #ARTIST fixed for &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="305"/>
        <source>#EDITION set to &quot;%1&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="315"/>
        <source>#GENRE set to &quot;%1&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="379"/>
        <source>#LANGUAGE changed from &quot;%1&quot; to &quot;%2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="395"/>
        <source>Apostrophes fixed for &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="424"/>
        <source>Line: %1, Note: %2, Diff: %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="431"/>
        <source>Potential duet with missing duet tags detected: &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="456"/>
        <source>Not a potential duet: &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Capitalization fix not applicable due to empty title: &quot;%1&quot;</source>
        <translation type="vanished">&quot;Wielką Literą&quot; nie może być użyte dla pustego tytułu: &quot;%1&quot;</translation>
    </message>
    <message>
        <source>Capitalization of #TITLE fixed for &quot;%1 - %2&quot;</source>
        <translation type="vanished">&quot;Wielką Literą&quot; użyte dla #TITLE (tytuł) dla &quot;%1 - %2&quot;</translation>
    </message>
    <message>
        <source>Capitalization fix not applicable due to empty artist: &quot;%1&quot;</source>
        <translation type="vanished">&quot;Wielką Literą&quot; nie może być użyte dla pustego wykonawcy: &quot;%1&quot;</translation>
    </message>
    <message>
        <source>Capitalization of #ARTIST fixed for &quot;%1 - %2&quot;</source>
        <translation type="vanished">&quot;Wielką Literą&quot; użyte dla #ARTIST (artysta) dla &quot;%1 - %2&quot;</translation>
    </message>
</context>
<context>
    <name>QUPreparatoryTaskFactory</name>
    <message>
        <location filename="QUPreparatoryTaskFactory.cpp" line="11"/>
        <source>Preparatory Tasks</source>
        <translation>Zadania przygotowawcze</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTaskFactory.cpp" line="15"/>
        <source>Fix general issues and prepare song for other tasks.</source>
        <translation>Ustalenie ogólnych zagadnień i przygotowanie utworu do innych zadań.</translation>
    </message>
</context>
<context>
    <name>QUSimpleTaskFactory</name>
    <message>
        <location filename="../shared/QUSimpleTaskFactory.h" line="16"/>
        <source>Simple Task</source>
        <translation>Proste zadanie</translation>
    </message>
</context>
<context>
    <name>QUSmartSetting</name>
    <message>
        <location filename="../shared/QUSmartSetting.cpp" line="30"/>
        <source>Reset to default value.</source>
        <translation>Przywróc udomyślne ustawienia.</translation>
    </message>
</context>
</TS>
