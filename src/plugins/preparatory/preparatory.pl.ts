<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="pl_PL">
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
        <source>Remove unsupported tags</source>
        <translation>Usuwanie nierozpoznanych etykiet</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="33"/>
        <source>Fix audio length via #END tag</source>
        <translation>Popraw długość utworu w oparciu o etykietę #END</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="34"/>
        <source>Sets &lt;b&gt;#END&lt;/b&gt; to the length of the song if audio file present and longer than song. &lt;b&gt;Appends a little time buffer.&lt;/b&gt;</source>
        <translation>Ustawia &lt;b&gt;#END&lt;/b&gt; z wpisem długości utworu, przydatne gdy plik z muzyką jest dłuższy od utworu. &lt;b&gt;Dodaje nieco czasu.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="38"/>
        <source>Round #GAP to nearest integer</source>
        <oldsource>Round #GAP to nearest integer.</oldsource>
        <translation>Przybliż #GAP do najbliższej liczby po przecinku</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="42"/>
        <source>Capitalize #TITLE tag</source>
        <oldsource>Fix capitalization of #TITLE tag</oldsource>
        <translation>&quot;Wielką Literą&quot; tytuł</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="43"/>
        <source>Applies some rules of capitalization to form a consistent appearance for all song titles.&lt;br&gt;&lt;br&gt;&lt;b&gt;Useful for english songs.&lt;/b&gt;</source>
        <translation>Funkcja &quot;Wielką Literą&quot; zmienia pierwsze małe litery na wielkie tworząc spójny wygląd tytułów piosenek.&lt;br&gt;&lt;br&gt;&lt;b&gt;Uzywaj do angielskich piosenek.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="47"/>
        <source>Capitalize #ARTIST tag</source>
        <translation>&quot;Wielką Literą&quot; wykonawca</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="48"/>
        <source>Capitalize each word of the artist, e.g. &quot;michael jackson&quot; -&gt; &quot;Michael Jackson&quot;.</source>
        <translation>&quot;Wielką Literą&quot; nazwa wykonawcy, np. &quot;sylwia grzeszczak&quot; -&gt; &quot;Sylwia Grzeszczak&quot;.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="93"/>
        <location filename="QUPreparatoryTask.cpp" line="94"/>
        <source>Pattern:</source>
        <translation>Wzór:</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="93"/>
        <source>(cover)</source>
        <translation>(okładka)</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="94"/>
        <source>(background)</source>
        <translation>(tło)</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="101"/>
        <source>Buffer:</source>
        <translation>Bufor:</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="101"/>
        <source>seconds</source>
        <translation>sekundy</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="104"/>
        <source>Capitalize English songs only</source>
        <translation>&quot;Wielką Literą&quot; tylko angielskie piosenki</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="105"/>
        <source>Capitalize each word</source>
        <translation>&quot;Wielką Literą&quot; każde słowo</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="106"/>
        <location filename="QUPreparatoryTask.cpp" line="109"/>
        <source>Capitalize first word only</source>
        <translation>&quot;Wielką Literą&quot; tylko pierwsze słowo</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="148"/>
        <source>Capitalization fix not applicable due to non-english song: &quot;%1 - %2&quot;. Try to configure the task.</source>
        <translation>&quot;Wielką Literą&quot; nie może być zastosowane do nieangielskich piosenek: &quot;%1 - %2&quot;. Spróbuj zmienić zadanie.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="156"/>
        <source>Capitalization fix not applicable due to empty title: &quot;%1&quot;</source>
        <translation>&quot;Wielką Literą&quot; nie może być użyte dla pustego tytułu: &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="221"/>
        <source>Capitalization of #TITLE fixed for &quot;%1 - %2&quot;</source>
        <translation>&quot;Wielką Literą&quot; użyte dla #TITLE (tytuł) dla &quot;%1 - %2&quot;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="234"/>
        <source>Capitalization fix not applicable due to empty artist: &quot;%1&quot;</source>
        <translation>&quot;Wielką Literą&quot; nie może być użyte dla pustego wykonawcy: &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="249"/>
        <source>Capitalization of #ARTIST fixed for &quot;%1 - %2&quot;</source>
        <translation>&quot;Wielką Literą&quot; użyte dla #ARTIST (artysta) dla &quot;%1 - %2&quot;</translation>
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
