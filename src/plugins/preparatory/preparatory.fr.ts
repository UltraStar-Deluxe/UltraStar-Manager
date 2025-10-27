<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr_FR">
<context>
    <name>QUMultilingualTaskFactory</name>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="25"/>
        <source>Unnamed plugin</source>
        <translation>Plugin sans nom</translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="27"/>
        <source>Unknown person</source>
        <translation>Personne inconnue</translation>
    </message>
    <message>
        <location filename="../shared/QUMultilingualTaskFactory.h" line="28"/>
        <source>No description given.</source>
        <translation>Pas de description disponible.</translation>
    </message>
</context>
<context>
    <name>QUPreparatoryTask</name>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="22"/>
        <source>Assign missing files automatically</source>
        <translation>Assigner les fichiers manquants automatiquement</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="23"/>
        <source>&lt;b&gt;#MP3&lt;/b&gt; ... first audio file found.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... first picture file, following a pattern.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... first picture file, following a pattern.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... first video file found.&lt;br&gt;&lt;br&gt;Does not reset tags that have already been set.</source>
        <translation>&lt;b&gt;#MP3&lt;/b&gt; ... premier fichier audio trouvé.&lt;br&gt;&lt;b&gt;#COVER&lt;/b&gt; ... première image suivant le modèle.&lt;br&gt;&lt;b&gt;#BACKGROUND&lt;/b&gt; ... première image suivant le modèle.&lt;br&gt;&lt;b&gt;#VIDEO&lt;/b&gt; ... premier fichier vidéo trouvé.&lt;br&gt;&lt;br&gt;Pas de remise à zéro des champs qui sont déjà fixés.</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="31"/>
        <source>Fix or remove unsupported tags</source>
        <oldsource>Remove unsupported tags</oldsource>
        <translation type="unfinished">Suppression des champs non supportés</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="32"/>
        <source>Replaces the incorrect &lt;b&gt;#AUTHOR&lt;/b&gt; tag by the correct &lt;b&gt;#CREATOR&lt;/b&gt; tag and deletes all other unsupported tags.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="36"/>
        <source>Fix audio length via #END tag</source>
        <translation>Corriger la longueur du fichier audio grâce au champ #END</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="37"/>
        <source>Sets &lt;b&gt;#END&lt;/b&gt; to the length of the song if audio file present and longer than song. &lt;b&gt;Appends a little time buffer.&lt;/b&gt;</source>
        <translation>Fixer &lt;b&gt;#END&lt;/b&gt; à la longueur de la chanson si le fichier audio présent est plus long que la chanson. &lt;b&gt;Nécessite un peu de temps pour mettre en mémoire tampon.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="41"/>
        <source>Round #GAP to nearest integer</source>
        <oldsource>Round #GAP to nearest integer.</oldsource>
        <translation>Arrondir #GAP à la valeur entière la plus proche</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="45"/>
        <source>Capitalize #TITLE tag</source>
        <oldsource>Fix capitalization of #TITLE tag</oldsource>
        <translation>Mettre en capitale le champ #TITLE</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="46"/>
        <source>Applies some rules of capitalization to form a consistent appearance for all song titles.&lt;br&gt;&lt;br&gt;&lt;b&gt;Useful for english songs.&lt;/b&gt;</source>
        <translation>Appliquer des règles de majuscule/minuscule pour créer une cohérence dans les titres des chansons.&lt;br&gt;&lt;br&gt;&lt;b&gt; Utile pour les chansons anglaises.&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="50"/>
        <source>Capitalize #ARTIST tag</source>
        <translation>Mettre les capitales dans le champ #ARTIST</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="51"/>
        <source>Capitalize each word of the artist, e.g. &quot;michael jackson&quot; -&gt; &quot;Michael Jackson&quot;.</source>
        <translation>Mettre des capitales à chaque mot du champ #ARTIST, par exemple &quot;michael jackson&quot; -&gt; &quot;Michael Jackson&quot;.</translation>
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
        <translation>Modèle:</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="144"/>
        <source>(cover)</source>
        <translation>(couverture)</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="145"/>
        <source>(background)</source>
        <translation>(fond d&apos;écran)</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="152"/>
        <source>Buffer:</source>
        <translation>Mémoire tampon:</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="152"/>
        <source>seconds</source>
        <translation>secondes</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="155"/>
        <source>Capitalize English songs only</source>
        <translation>Ne mettre des capitales que dans les chansons anglaises</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="156"/>
        <source>Capitalize each word</source>
        <translation>Mettre des capitales à tous les mots</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTask.cpp" line="157"/>
        <location filename="QUPreparatoryTask.cpp" line="160"/>
        <source>Capitalize first word only</source>
        <translation>Mettre des capitales qu&apos;aux premiers mots</translation>
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
        <translation>La correction des capitales n&apos;est pas applicable à &quot;%1 - %2&quot; car ce n&apos;est pas une chanson anglaise. Essayez de configurer la tâche.</translation>
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
        <translation type="vanished">La correction des capitales n&apos;est pas applicable à &quot;%1&quot; car le titre est vide</translation>
    </message>
    <message>
        <source>Capitalization of #TITLE fixed for &quot;%1 - %2&quot;</source>
        <translation type="vanished">La mise en capitale du champ #TITLE a été corrigée pour &quot;%1 - %2&quot;</translation>
    </message>
    <message>
        <source>Capitalization fix not applicable due to empty artist: &quot;%1&quot;</source>
        <translation type="vanished">La correction des capitales n&apos;est pas applicable à &quot;%1&quot; car le nom de l&apos;artiste est vide</translation>
    </message>
    <message>
        <source>Capitalization of #ARTIST fixed for &quot;%1 - %2&quot;</source>
        <translation type="vanished">La mise en capitale du champ #ARTIST a été corrigée pour &quot;%1 - %2&quot;</translation>
    </message>
</context>
<context>
    <name>QUPreparatoryTaskFactory</name>
    <message>
        <location filename="QUPreparatoryTaskFactory.cpp" line="11"/>
        <source>Preparatory Tasks</source>
        <translation>Préparation des tâches</translation>
    </message>
    <message>
        <location filename="QUPreparatoryTaskFactory.cpp" line="15"/>
        <source>Fix general issues and prepare song for other tasks.</source>
        <translation>Résolvez des problèmes généraux et préparez la chanson pour d&apos;autres tâches.</translation>
    </message>
</context>
<context>
    <name>QUSimpleTaskFactory</name>
    <message>
        <location filename="../shared/QUSimpleTaskFactory.h" line="16"/>
        <source>Simple Task</source>
        <translation>Tâche simple</translation>
    </message>
</context>
<context>
    <name>QUSmartSetting</name>
    <message>
        <location filename="../shared/QUSmartSetting.cpp" line="30"/>
        <source>Reset to default value.</source>
        <translation>Réinitialiser les valeurs par défaut.</translation>
    </message>
</context>
</TS>
