<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="pt_PT">
<context>
    <name>QUMultilingualTaskFactory</name>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="25"/>
        <source>Unnamed plugin</source>
        <translation>Plugin sem nome</translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="27"/>
        <source>Unknown person</source>
        <translation>Pessoa desconhecida</translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="28"/>
        <source>No description given.</source>
        <translation>Sem descrição.</translation>
    </message>
</context>
<context>
    <name>QUPreparatoryTask</name>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="22"/>
        <source>Assign missing files automatically</source>
        <translation>Atribuir ficheiros ausentes automaticamente</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="23"/>
        <source>&lt;b&gt;#MP3&lt;/b&gt; ... first audio file found.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... first picture file, following a pattern.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... first picture file, following a pattern.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... first video file found.&lt;br&gt;&lt;br&gt;Does not reset tags that have already been set.</source>
        <translation>&lt;b&gt;#MP3&lt;/b&gt; ... primeiro ficheiro audio encontrado.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... primeiro ficheiro imagem, seguindo um padrão.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... primeiro ficheiro imagem, seguindo um padrão.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... primeiro ficheiro video encontrado.&lt;br&gt;&lt;br&gt;Não repor etiquetas já definidas.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="31"/>
        <source>Fix or remove unsupported tags</source>
        <oldsource>Remove unsupported tags</oldsource>
        <translation type="unfinished">Remover etiquetas não suportadas</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="32"/>
        <source>Replaces the incorrect &lt;b&gt;#AUTHOR&lt;/b&gt; tag by the correct &lt;b&gt;#CREATOR&lt;/b&gt; tag and deletes all other unsupported tags.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="36"/>
        <source>Fix audio length via #END tag</source>
        <translation>Corrigir comprimento de audio via etiqueta #END</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="37"/>
        <source>Sets &lt;b&gt;#END&lt;/b&gt; to the length of the song if audio file present and longer than song. &lt;b&gt;Appends a little time buffer.&lt;/b&gt;</source>
        <translation>Definir etiqueta &lt;b&gt;#END&lt;/b&gt; ao comprimento da canção se ficheiro audio presente e maior que canção. &lt;b&gt;Adicionar pequeno tempo de buffer.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="41"/>
        <source>Round #GAP to nearest integer</source>
        <oldsource>Round #GAP to nearest integer.</oldsource>
        <translation>Arredondar #GAP para o inteiro mais próximo</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="45"/>
        <source>Capitalize #TITLE tag</source>
        <oldsource>Fix capitalization of #TITLE tag</oldsource>
        <translation>Capitalizar etiqueta #TITLE</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="46"/>
        <source>Applies some rules of capitalization to form a consistent appearance for all song titles.&lt;br&gt;&lt;br&gt;&lt;b&gt;Useful for english songs.&lt;/b&gt;</source>
        <translation>Aplicar algumas regras de capitalização para formar uma aparência consistente para todas as canções.&lt;br&gt;&lt;br&gt;&lt;b&gt;Útil para as canções inglesas.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="50"/>
        <source>Capitalize #ARTIST tag</source>
        <translation>Capitalizar etiqueta #ARTIST</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="51"/>
        <source>Capitalize each word of the artist, e.g. &quot;michael jackson&quot; -&gt; &quot;Michael Jackson&quot;.</source>
        <translation>Capitalizar cada palavra do artista, ex: &quot;michael jackson&quot; -&gt; &quot;Michael Jackson&quot;.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="55"/>
        <source>Set #EDITION tag if it is empty</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="56"/>
        <source>Sets &lt;b&gt;#EDITION&lt;/b&gt; to a defined value if it is empty or not present.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="60"/>
        <source>Set #GENRE tag if it is empty</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="61"/>
        <source>Sets &lt;b&gt;#GENRE&lt;/b&gt; to a defined value if it is empty or not present.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="65"/>
        <source>Fix #LANGUAGE tag to contain English language names</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="66"/>
        <source>Sets &lt;b&gt;#LANGUAGE&lt;/b&gt; to contain English language names (e.g. Deutsch -&gt; German).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="70"/>
        <source>Fix apostrophes in song artist and title</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="71"/>
        <source>Replaces wrongfully used apostrophe symbols `, ´, &apos; by the correct apostrophe ’</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="75"/>
        <source>Add missing ’P1’ and ’P2’ in duets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="76"/>
        <source>Detects if a song is a duet that is missing ’P1’ and ’P2’ tags and adds them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="80"/>
        <source>Fix line endings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="81"/>
        <source>Change the .txt line endings to either CRLF or LF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="144"/>
        <location filename="QUPreparatoryTask.cpp" line="145"/>
        <source>Pattern:</source>
        <translation>Padrão:</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="144"/>
        <source>(cover)</source>
        <translation>(capa)</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="145"/>
        <source>(background)</source>
        <translation>(fundo)</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="152"/>
        <source>Buffer:</source>
        <translation>Buffer:</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="152"/>
        <source>seconds</source>
        <translation>segundos</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="155"/>
        <source>Capitalize English songs only</source>
        <translation>Capitalizar apenas canções inglesas</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="156"/>
        <source>Capitalize each word</source>
        <translation>Capitalizar cada palavra</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="157"/>
        <location filename="QUPreparatoryTask.cpp" line="160"/>
        <source>Capitalize first word only</source>
        <translation>Capitalizar apenas primeira palavra</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="163"/>
        <source>Edition:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="166"/>
        <source>Genre:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="169"/>
        <source>Convert to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="208"/>
        <source>Capitalization fix not applicable due to non-english song: &quot;%1 - %2&quot;. Try to configure the task.</source>
        <translation>Corrigir capitalização não aplicável, devido às canções não-inglesas: &quot;%1 -%2&quot;. Tente configurar a tarefa.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="216"/>
        <source>Capitalization fix not applicable due to empty title: &quot;%1&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="281"/>
        <source>Capitalization of #TITLE fixed for &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="294"/>
        <source>Capitalization fix not applicable due to empty artist: &quot;%1&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="309"/>
        <source>Capitalization of #ARTIST fixed for &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="318"/>
        <source>#EDITION set to &quot;%1&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="328"/>
        <source>#GENRE set to &quot;%1&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="392"/>
        <source>#LANGUAGE changed from &quot;%1&quot; to &quot;%2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="408"/>
        <source>Apostrophes fixed for &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="437"/>
        <source>Line: %1, Note: %2, Diff: %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="444"/>
        <source>Potential duet with missing duet tags detected: &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="469"/>
        <source>Not a potential duet: &quot;%1 - %2&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Capitalization fix not applicable due to empty title: &quot;%1&quot;</source>
        <translation type="vanished">Corrigir capitalização não aplicável, devido a título vazio: &quot;%1&quot;</translation>
    </message>
    <message>
        <source>Capitalization of #TITLE fixed for &quot;%1 - %2&quot;</source>
        <translation type="vanished">Capitalização de #TITLE corrigida para &quot;%1&apos;-&apos;%2&quot;</translation>
    </message>
    <message>
        <source>Capitalization fix not applicable due to empty artist: &quot;%1&quot;</source>
        <translation type="vanished">Corrigir capitalização não aplicável, devido a artista vazio: &quot;%1&quot;</translation>
    </message>
    <message>
        <source>Capitalization of #ARTIST fixed for &quot;%1 - %2&quot;</source>
        <translation type="vanished">Capitalização de #ARTIST corrigida para &quot;%1&apos;-&apos;%2&quot;</translation>
    </message>
</context>
<context>
    <name>QUPreparatoryTaskFactory</name>
    <message>
        <location filename="QUPreparatoryTaskFactory.cpp" line="11"/>
        <source>Preparatory Tasks</source>
        <translation>Tarefas Preparatórias</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTaskFactory.cpp" line="15"/>
        <source>Fix general issues and prepare song for other tasks.</source>
        <translation>Corrigir problemas gerais e preparar canção para outras tarefas.</translation>
    </message>
</context>
<context>
    <name>QUSimpleTaskFactory</name>
    <message>
        <location filename="../shared/QUSimpleTaskFactory.h" line="16"/>
        <source>Simple Task</source>
        <translation>Tarefa Simples</translation>
    </message>
</context>
<context>
    <name>QUSmartSetting</name>
    <message>
        <location filename="../shared/QUSmartSetting.cpp" line="30"/>
        <source>Reset to default value.</source>
        <translation>Repor para o valor padrão.</translation>
    </message>
</context>
</TS>
