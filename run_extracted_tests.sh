#!/bin/bash

# main.valaから抽出されたテストコマンドを実行するスクリプト

cd "/home/chinachu/42/minishell_42"

if [ ! -f "./minishell" ]; then
    echo "Error: minishell not found. Please run 'make' first."
    exit 1
fi

test_count=0
pass_count=0
fail_count=0

echo "=== Running Extracted Test Commands ==="
echo ""

run_test() {
    local cmd="$1"
    test_count=$((test_count + 1))
    echo "=== Test $test_count ==="
    echo -n "Command: "
    # 改行文字を視覚的に表示
    echo "$cmd" | sed ':a;N;$!ba;s/\n/\\n/g'
    echo "Output:"
    
    # コマンドをminishellに送信
    printf "%s\n" "$cmd" | timeout 10 ./minishell
    
    local exit_code=$?
    if [ $exit_code -eq 0 ] || [ $exit_code -eq 1 ]; then
        pass_count=$((pass_count + 1))
        echo "Result: PASS"
    else
        fail_count=$((fail_count + 1))
        echo "Result: FAIL (exit code: $exit_code)"
    fi
    echo ""
}

run_test \|
run_test \|\ echo\ oi
run_test \|\ \|
run_test \|\ \|\ \|
run_test \|\ \$
run_test \>
run_test \>\>
run_test \>\>\>
run_test \<
run_test \<\<
run_test echo\ hi\ \<
run_test cat\ \ \ \ \<\|\ ls
run_test echo\ hi\ \|\ \>
run_test echo\ hi\ \|\ \>\ \>\>
run_test echo\ hi\ \|\ \<\ \|
run_test echo\ hi\ \|\ \ \ \|\ 
run_test echo\ \$EMPYT
run_test echo\ \$EMPYT\ \ \ \ abc
run_test echo\ \$EMPYT\ abc
run_test echo\ \$EMPYT\ abc\ \ \ \$EMPTY
run_test echo\ \$EMPYT\ abc\ \$EMPTY
run_test \$EMPTY\ echo\ \$EMPYT\ abc
run_test \$
run_test ..\ \|\ ..\ \|\ ..
run_test ./no_permission
run_test \ /bin/ls\ 
run_test \ /bin/ls\ -la\ 
run_test \ /bin/ls\ -l\ 
run_test \ /bin/ls\ -l\ -a\ 
run_test \ /bin/ls\ -l\ -a\ -t\ 
run_test \ printf\ \"Hello\ World\"\ 
run_test \ printf\ \'Hello\ World\'\ 
run_test \ echo\ \"Hello\ World\"\ 
run_test \ echo\ \'Hello\ World\'\ 
run_test \ echo\ \"Hello\ World\"\ \|\ cat\ -e\ 
run_test \ printf\ \'Hello\ World\'\ \|\ cat\ -e\ 
run_test \ echo\ \"Hello\ World\"\ \|\ cat\ -e\ \|\ cat\ -e\ 
run_test \ echo\ \"cat\ Makefile\ \|\ cat\ \>\ \$USER\"\ \ \ 
run_test \ echo\ \'cat\ Makefile\ \|\ cat\ \>\ \$USER\'\ \ \ 
run_test \ /bin/ls\ \|\ /usr/bin/cat\ -e\ 
run_test \ echo\ \'\ \'\ 
run_test \ echo\ \'\ \ \ \ \'\ 
run_test \ echo\ \'\ \ \ \ \ \ \ \ \'\ 
run_test \ echo\ \"\ \ \ \ \ \ \ \ \"\ 
run_test \ echo\ \"\ \ \ \ \"\ 
run_test \ echo\ \"\ \"\ 
run_test echo\ hello\'world\'
run_test echo\ hello\"\"world
run_test echo\ \'\'
run_test echo\ \"\$PWD\"
run_test echo\ \'\$PWD\'
run_test echo\ \"\>\ \>\>\ \<\ \*\ \?\ \[\ \]\ \|\ \;\ \[\ \]\ \|\|\ \&\&\ \(\ \}\)\ \&\ #\ \$\ \ \<\<\"
run_test echo\ \'\>\ \>\>\ \<\ \*\ \?\ \[\ \]\ \|\ \;\ \[\ \]\ \|\|\ \&\&\ \(\ \}\)\ \&\ #\ \$\ \ \<\<\'
run_test echo\ \"exit_code\ -\>\$\?\ user\ -\>\$USER\ home\ -\>\ \$HOME\"
run_test echo\ \'exit_code\ -\>\$\?\ user\ -\>\$USER\ home\ -\>\ \$HOME\'
run_test echo\ \"\$\"
run_test echo\ \'\$\'
run_test echo\ \$
run_test echo\ \$\?
run_test echo\ \$\?HELLO
run_test \ printf\ \ \$\"\ \ \$USER\ \ a\"\ \ 
run_test \ 
run_test \"\"
run_test \ cat\ -e\ \<\ Makefile\ \<\ ../.gitignor\ \ 
run_test \ \<\ file_not_found\ 
run_test \ \<\ file_not_found\ cat
run_test \ \<\ file_not_found\ \>\ /dev/stdout
run_test \ echo\ -n\ salut
run_test \ echo\ -nnnnnnnnnnnnnn1\ salut
run_test \ echo\ -nnnnnnnnnnnnnn1\ salut\ \ nvobwriov\ wr\ gow\ v\ \'\$USER\>eef\>\$USER\'\ 
run_test \ echo\ -nnnnnnnnnnnnnnnnnnn1\ salut\ \ nvobwriov\ wr\ gow\ v\ \'\$USER\>eef\>\$USER\'\ \ 
run_test \ echo\ -nnnnnnnnnnnnnnnnnnn1\ salut\ \ nvobwriov\ wr\ gow\ v\ \'\>p\'\ 
run_test \ cat\ -e\ \<\ Makefile\ \<\ ../.gitignore\ 
run_test \ ls\ -l\ \|\ cat\ -e\ \ \|\ ls\ 
run_test \ printf\ \'Syntax\ Error\!\'\ \|\ \|\ ls\ 
run_test \ printf\ \'Syntax\ Error\!\'\ \<\ \|\ ls\ 
run_test \ printf\ \'Syntax\ Error\!\'\ \ \>\>\ \|\ ls\ 
run_test \ printf\ \'Syntax\ Error\!\'\ \|\ \>\ file_out\ 
run_test \ printf\ \'Syntax\ Error\!\'\ \|\>\ file_out\ 
run_test \ \>x\ printf\ \'Syntax\ Error\!\'\ \|\ 
run_test \ \|\ \>x\ printf\ \'Syntax\ Error\!\'\ 
run_test \ \>x\ printf\ \'Syntax\ Error\!\'\ \>\ 
run_test \ \>x\ printf\ \'Syntax\ Error\!\'\ \<\<\ 
run_test \ echo\ \'\>\'\ test\ \'\<\'\ 
run_test \ echo\ \'\>\'
run_test \ echo\ \'\<\'
run_test \ echo\ \'\<\<\'
run_test \ echo\ \'\>\>\'
run_test \ echo\ \'\>test\<\'\ 
run_test \ echo\ \'\>test\'\ 
run_test \ echo\ \'test\<\'\ 
run_test \ echo\ \"\|\"\ 
run_test \ echo\ \'\>test\ \'\<\ 
run_test \ echo\ \'\ test\ \<\'\ 
run_test \ echo\ \"\>\"\ 
run_test \ echo\ \"\>test\<\"\ 
run_test \ echo\ \"\>test\"\ 
run_test \ echo\ \"test\<\"\ 
run_test \ printf\ \'Syntax\ Error\!\'\ \|\ \|\ ls\ 
run_test \ printf\ \'Syntax\ Error\!\'\ \<\ \|\ ls\ 
run_test \ printf\ \'Syntax\ Error\!\'\ \ \>\>\ \|\ ls\ 
run_test \ printf\ \'Syntax\ Error\!\'\ \|\ \>\ file_out\ 
run_test \ printf\ \'Syntax\ Error\!\'\ \|\>\ file_out\ 
run_test \ \>x\ printf\ \'Syntax\ Error\!\'\ \|\ 
run_test \ \|\ \>x\ printf\ \'Syntax\ Error\!\'\ 
run_test \ \>x\ printf\ \'Syntax\ Error\!\'\ \>\ 
run_test \ \>x\ printf\ \'Syntax\ Error\!\'\ \<\<\ 
run_test \ echo\ \'\>\'\ test\ \'\<\'\ 
run_test \ echo\ \'\>\'
run_test \ echo\ \'\>test\<\'\ 
run_test \ echo\ \'\>test\'\ 
run_test \ echo\ \'test\<\'\ 
run_test \ echo\ \"\|\"\ 
run_test \ echo\ \'\>test\ \'\<\ 
run_test \ echo\ \'\ test\ \<\'\ 
run_test \ echo\ \"\>\"\ 
run_test \ echo\ \"\>test\<\"\ 
run_test \ echo\ \"\>test\"\ 
run_test \ echo\ \"test\<\"\ 
run_test \ echo\ hello\|cat\ -e
run_test \ echo\ hello\ \ \ \ \ \ \|cat\ -e
run_test \ echo\ hello\|\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ cat\ -e
run_test \ echo\ \ \ \ \ \ \ \ \ hello\ \ \ \ \ \ \ \ \|\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ cat\ -e
run_test \ ls\|ls\|ls\|ls\|ls\|ls\|cat\ \ \ \ -e
run_test \ ls\|ls\|ls\|ls\|ls\|ls\|ls\|ls\ \|ls\ \|\ ls\ \ \ \ \|\ ls\ \|\ ls\ \|\ cat\ \ \ \ -e
run_test \ command_not_found\ \|\ echo\ \'abc\'
run_test \ command_not_found\ \|\ cat
run_test \ cat\ Makefile\ \|\ cat\ /dev/urandom\ \|\ ls\ \|\ wc\ -w\ 
run_test \ ls\ \|\ ls\ \|ls\ \|\ ls\|\ ls\ \|ls\|\ ls\|ls\ \|\ ls\ \|\ ls\ \|\ ls\ \|\ ls
run_test \ cat\ Makefile\ \|\ md5sum\ 
run_test \ cat\ Makefile\ \|\ grep\ -o\ SRC\ \|\ tr\ \'n\'\ \'\ \'\ 
run_test \ cat\ /dev/urandom\ \|\ head\ -c\ 15\ \|\ wc\ -c\ 
run_test \ cat\ /dev/urandom\ \|\ strings\ \|\ grep\ -o\ \"\[A-Z\]\[0-9\]\"\ \|\ tr\ -d\ \'n\'\ \|\ head\ -c\ 15\ \|\ wc\ -c\ 
run_test \ echo\ \$\ 
run_test \ echo\ \$\ USER\ 
run_test \ echo\ \$USER\ 
run_test \ echo\ \$USER\$\ 
run_test \ echo\ \$USER\ \$\ 
run_test \ echo\ \"\$USER\$\"\ 
run_test \ echo\ \"\$USER\ \$\"\ 
run_test \ echo\ \$JENEXISTEPAS\ 
run_test \ echo\ \$\ JENEXISTEPAS\ 
run_test \ echo\ \$USER_\$USER\ 
run_test \ echo\ \$USER\$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ \$USER\ 
run_test \ printf\ \"\$USERn\"\ 
run_test \ printf\ \$\?\ 
run_test \ printf\ \$\?\?\ 
run_test \ printf\ \$\?\?\?\ \$\?\?\ \$\?\ 
run_test \ printf\ \"\$USER\$USER\'\'\ =\ \'\ \$L\ ANG\ \'\"\ \'\'\ 
run_test \ printf\ \"\$USER\$USER\"\ 
run_test \ echo\ \$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\ \|\ wc\ -l
run_test \ echo\ \$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\ \|\ wc\ -l
run_test \ echo\ \$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\ \|\ wc\ -l
run_test \ echo\ \$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\$\ \|\ wc\ -l
run_test \ printf\ \"\$USER\$USER\'\'\ =\ \'\ \$L\ ANG\ \'\"\ 
run_test \ printf\ \"=\ \ PATH\ \ \ \ \"\ \ \'t\$USER\ \ \'\ \"\"\ 
run_test \ prin\"tf\ \"\$USER\ 
run_test \ pri\"tf\"\ \$USER\ 
run_test \ pri\"tf\ \$USER\"\ 
run_test \ \"printf\"\ \$USER\ 
run_test \ \"printf\"\ \"\"\$USER\"bj\'r\'\"\ 
run_test \ \"pr\'int\'f\"\ \"\"\$\'USER\'\"bj\'r\'\"\ 
run_test \ printf\ \$\'hello\'\ 
run_test \ printf\ \$\"hello\"\ 
run_test \ \'\'\'\'\'\'\"/bin/ls\"\'\'\'\'\'\'\ 
run_test \ /b\'i\"n/ls\'\ -l\ \ 
run_test \ /b\'\'i\"n/\'\'ls\ -\"na\'\"th\'an
run_test \ /b\'i\'\'\'\'\'\'\'\'n/\"l\'s\ 
run_test \ /b\'in/l\'s\ -l\'a\'
run_test \ \'/\'b\"in/l\"s\ 
run_test \ echo\ \'\$\?\'\ 
run_test \ echo\ \'\'\$USER\'\'\ 
run_test \ echo\ \'\'\'\$USER\'\'\'\ 
run_test \ echo\ 
run_test \ echo\ \'\'\ b
run_test \ echo\ \'\'\ \'\'x
run_test \ echo\ \'\'\ \"\'\'\"\ \'\'X
run_test \ echo\$USER\ 
run_test \ echo\ -n\ \$USER\ -n\ hello\ supra\ \$USER
run_test \ echo\ -n\ test
run_test \ echo\ test\ -n\ 
run_test \ echo\ -nnnnnnnnnnnnnn\ -nns\ -n\ test\ 
run_test \ echo\ -nnnnnnnnnnnnnn\ -nnn\ -n\ test\ 
run_test \ echo\ str1\ \ \"\"\ str3\"\ \ not\ finished\ yet\ ........\ done.\"\ 
run_test \ echo\ \ Ichi\ Ni\ San\ Yon\ Go\ \|\ cat\ -e
run_test \ \<\ Makefile\ cat\ \|\ md5sum\ 
run_test \ \<\ file_not_found\ cat\ \|\ wc\ -c\ 
run_test \ \<\ Makefile\ cmd_not_found\ \|\ wc\ -c\ 
run_test \ \<\ Makefile\ \|\ printf\ \'You\ see\ me\?\'\ 
run_test \ \<\ Makefile\ wc\ -w\ \ \<\ tester.vala\ 
run_test \ wc\ -w\ \<\ tester.vala\ \<\ Makefile\ 
run_test \ ls\ \|\ wc\ -w\ \<\ tester.vala\ \<\ Makefile\ 
run_test \ \<\ Makefile\ \<\ tester.vala\ wc\ -c\ 
run_test \ ls\ \|\ \<\ Makefile\ \<\ tester.vala\ wc\ -c\ 
run_test \ printf\ \'Hello\ World\'\ \>trash/\$WHOAMI.a.test\ \"\,\ \"cat\ trash/\$WHOAMI.a.test\ -e
run_test \ \>trash/\$WHOAMI.b.test\ printf\ \'Hello\ World\'\ \>trash/\$WHOAMI.c.test\ \"\,\ \"cat\ trash/\$WHOAMI.b.test\ -e\"\,\ \"echo\ A\"\,\ \"cat\ trash/\$WHOAMI.c.test\ -e
run_test \$PWD
run_test \$EMPTY
run_test \$EMPTY\ echo\ hi
run_test ./test_files/invalid_permission
run_test ./missing.out
run_test missing.out
run_test \"aaa\"
run_test \'aaa\'
run_test test_files
run_test ./test_files
run_test /test_files
run_test minishell.h
run_test \$
run_test \$\?
run_test README.md
run_test edsfdsf\"\ \,\ \"echo\ error:\ \$\?
run_test clear\ \|\ pwd
run_test clear\ \|\ pwd\ \|\ cat\ -e
run_test clear\ \|\ pwd\ .\ \|\ cat\ -e
run_test pwd
run_test pwd\ ..
run_test pwd\ .
run_test pwd\ .\ \|\ cat\ -e
run_test pwd\ \|\ cat\ -e
run_test printf\ a\ \|\ pwd\ \|\ cat\ -e
run_test unset\ LD_PRELOAD\"\,\ \"unset\ WHOAMI\"\,\ \"unset\ PWD\"\,\ \"unset\ LANG\"\,\ \"unset\ OLDPWD\"\,\ \"unset\ ARGS\"\,\ \"unset\ MAKEFLAGS\"\,\ \"unset\ MFLAGS\"\,\ \"env\ \|\ grep\ -v\ _\ \|\ grep\ -v\ SHLVL\ \|\ grep\ -v\ SHELL\ \|\ sort
run_test \ env\ \|\ grep\ USER\ \|\ md5sum\ 
run_test \ env\ \|\ grep\ USER\ 
run_test unset\ 6_a
run_test unset\ ndacunh
run_test unset\ 0oui
run_test unset\ PWD\ HERE\"\,\ \"echo\ \$PWD
run_test export\ ABC\"\,\ \"env\ \|\ grep\ ABC\ -o\"\,\ \"printf\ hi\"\,\ \"env\ \|\ grep\ ABC
run_test export\ NDACUNH=42\"\,\ \"unset\ NDACUNH\ \|\ printf\ hey\"\,\ \"printf\ :\ \$NDACUNH
run_test export\ GHOST=123\ \|\ env\ \|\ grep\ GHOST
run_test export\ hello
run_test \"\"export\ HELLO=123\"\"\"\,\ \"printf\ :\ \$HELLO
run_test export\ A-
run_test export\ HELLO=123\ A
run_test export\ HELLO=\"123\ A-\"
run_test export\ hello\ world
run_test export\ HELLO-=123
run_test export\ =
run_test export\ 123
run_test \"\"export\ SLS=\'/bin/ls\'\"\"\"\,\ \"printf\ here:\"\,\ \"\$SLS
run_test export\ UNO=1\ DOS-2\ TRES=3\ \|\ env\ \|\ grep\ TRES
run_test export\ abcd=abcd\"\,\ \"export\ abcd+=ndacunh\"\,\ \"env\ \|\ grep\ abcd
run_test export\ ABCD=abcd\"\,\ \"export\ ABCD\ +=ndacunh\"\,\ \"env\ \|\ grep\ ABCD
run_test export\ ABCD=abcd\"\,\ \"export\ ABCD\ +=\ ndacunh\"\,\ \"env\ \|\ grep\ ABCD
run_test export\ ABCD=abcd\"\,\ \"export\ ABCD+=\ ndacunh\"\,\ \"env\ \|\ grep\ ABCD
run_test export\ ABCD\ =abcd\"\,\ \"env\ \|\ grep\ ABCD
run_test export\ ABCD=\ abcd\"\,\ \"env\ \|\ grep\ ABCD
run_test export\ ABCD=Hello\"\,\ \"export\ ABCD\ =abcd\"\,\ \"env\ \|\ grep\ ABCD
run_test export\ ABCD=Hello\"\,\ \"export\ ABCD=\ abcd\"\,\ \"env\ \|\ grep\ ABCD
run_test unset\ HELLO=
run_test unset
run_test unset\ HELLO1\ HELLO2
run_test unset\ HOME\"\,\ \"echo\ \$HOME
run_test unset\ PATH\"\,\ \"/bin/ls
run_test unset\ PATH\"\,\ \"ls
run_test unset\ PATH\"\,\ @\"export\ PATH=\'\$PWD\'\"\,\ \"ls
run_test unset\ PATH\"\,\ @\"export\ PATH=\'\$PWD\'\"\,\ \"/bin/ls
run_test unset\ PATH\"\,\ @\"export\ PATH=\'\$PWD\'\"\,\ @\"\$PWD/ls
run_test export\ A=\'suprapack\'\"\,\ \"echo\ a\ \$A\"\,\ \"unset\ A\"\,\ \"echo\ a\ \$A
run_test export\ HELLO=abc\"\,\ \"unset\ HELLO
run_test export\ HELLO=abc\"\,\ \"unset\ HELL\"\,\ \"unset\ HELLOO\"\,\ \"printf\ :\ \$HELLO
run_test exit\ 123
run_test exit\ 298
run_test exit\ 256
run_test exit\ +100
run_test exit\ \"+100\"
run_test exit\ +\"100\"
run_test exit\ -100
run_test exit\ \"-100\"
run_test exit\ -\"100\"
run_test exit\ hello
run_test exit\ 42\ world
run_test exit\ 9223372036854775807
run_test exit\ 9223372036854775808
run_test exit\ -9223372036854775807
run_test exit\ -9223372036854775808
run_test exit\ -9223372036854775809
run_test \<\<\ end\ cat\ -e\ nsimplentestnend
run_test \<\<\ AH\ cat\ -e\ nsimplentestnendnAH
run_test \<\<\ AH\ cat\ -e\ nsimplennnnnnnendnAH
run_test \<\<\ AH\ cat\ -e\ \|\ grep\ -o\ \'simple\'\ nsimplenendnAH
run_test \<\<\ EOF\ cat\ -e\ n\$USERnEOF
run_test \<\<\ EOF\ cat\ -e\ nnda-cunhnEOF
run_test cat\ \<\<\ here\ -enhellonhere
run_test cat\ \<\<\ here\ -enhellonhere
run_test \*
run_test \*/\*
run_test \*/\*
run_test echo\ \*
run_test echo\ D\*
run_test echo\ \*Z
run_test echo\ \*t\ hola
run_test echo\ \*t
run_test echo\ \$\*
run_test echo\ hola\*hola\ \*
run_test echo\ \$hola\*
run_test echo\ \$HOME\*
run_test cd\ \*
run_test cd\ \*
run_test cd\ \*
run_test echo\ hola\ \|\|\ cat
run_test pwd\ \|\|\ ls
run_test echo\ hola\ \|\|\ echo\ bonjour
run_test echo\ hola\ \&\&\ echo\ bonjour
run_test echo\ bonjour\ \|\|\ echo\ hola
run_test echo\ bonjour\ \&\&\ echo\ hola
run_test echo\ -n\ bonjour\ \&\&\ echo\ -n\ hola
run_test pwd\ \&\&\ ls\ \&\&\ echo\ hola
run_test pwd\ \|\|\ ls\ \&\&\ echo\ hola
run_test pwd\ \&\&\ ls\ \|\|\ echo\ hola
run_test pwd\ \|\|\ ls\ \|\|\ echo\ hola
run_test ls\ \|\|\ export\ \"\"
run_test export\ \"\"\ \|\|\ ls
run_test ls\ \&\&\ export\ \"\"
run_test export\ \"\"\ \&\&\ ls
run_test export\ \"\"\ \&\&\ unset\ \"\"
run_test ls\ \|\|\ \;
run_test \;\ \|\|\ ls
run_test ls\ \&\&\ \;
run_test \;\ \&\&\ ls
run_test ls\ \|\|\ \<
run_test ls\ \&\&\ \<
run_test cat\ \|\ echo\ \|\|\ ls
run_test cat\ \|\ echo\ \&\&\ ls
run_test ls\ \|\|\ cat\ \|\ echo
run_test ls\ \&\&\ cat\ \|\ echo
run_test \(ls\)
run_test \(\ \(\ ls\ \)\ \)
run_test \(\ \(\ \)\ ls\ \)
run_test ls\ \&\&\ \(ls\)
run_test \(ls\ \&\&\ pwd\)
run_test \(\ \(\ ls\&\&pwd\ \)\ \)
run_test \(\ \(\ ls\ \)\ \&\&pwd\ \)
run_test \(ls\ \&\&\ \(\ \(\ pwd\ \)\ \)\ \)
run_test \(ls\ \&\&\ pwd\)\ \>\ hola\ \$\>\ cat\ hola
run_test \>\ hola\ ls\ \&\&\ pwd
run_test \>\ hola\ \(ls\ \&\&\ pwd\)
run_test \(\>\ pwd\)\ \$\>\ ls
run_test \(\<\ pwd\)\ \$\>\ ls
run_test \(\<\ pwd\)
run_test \(\ \(\ \(\ \(\ \(\ pwd\)\ \)\ \)\ \)\ \)
run_test \(\)\ pwd
run_test \>\ pwd\ \(ls\)
run_test \(ls\|\|pwd\)\&\&\(ls\|\|pwd\)
run_test \(lss\|\|pwd\)\&\&\(lss\|\|pwd\)
run_test \(lss\&\&pwd\)\&\&\(lss\&\&pwd\)
run_test \(ls\ \&\&\ pwd\ \|\ wc\)\ \>\ hola\ \$\>\ cat\ hola
run_test \(ls\ \&\&\ pwd\ \|\ wc\)\ \>\ hola\ \$\>\ \(ls\ \&\&\ pwd\ \|\ wc\)\ \>\ hola\ \$\>\ cat\ hola
run_test \(ls\ \&\&\ pwd\ \|\ wc\)\ \>\>\ hola\ \$\>\ echo\ hey\&\&\(ls\ \&\&\ pwd\ \|\ wc\)\ \>\ hola\ \$\>\ cat\ hola
run_test \(pwd\ \|\ wc\)\ \<\ hola
run_test \(ls\ \&\&\ pwd\ \|\ wc\)\ \<\ hola
run_test \(ls\ -z\ \|\|\ pwd\ \|\ wc\)\ \<\ hola
run_test echo\ hey\ \>\ hola\ \$\>\ \(pwd\ \|\ wc\)\ \<\ hola
run_test echo\ hey\ \>\ hola\ \$\>\ \(ls\ \&\&\ pwd\ \|\ wc\)\ \<\ hola
run_test echo\ hey\ \>\ hola\ \$\>\ \(ls\ -z\ \|\|\ pwd\ \|\ wc\)\ \<\ hola
run_test \(ls\ -z\ \|\|\ pwd\ \&\&\ ls\)
run_test ls\ \|\|\ \(cat\ Makefile\|grep\ srcs\)\ \&\&\ \(pwd\|wc\)
run_test ls\ -z\ \&\&\ \(ls\)\ \&\&\ \(pwd\)
run_test \(ls\ \>\ Docs/hey\ \&\&\ pwd\)\ \>\ hola\ \$\>\ cat\ hola\ \$\>\ cat\ Docs/hey
run_test ls\ \>\ Docs/hey\ \&\&\ pwd\ \>\ hola\ \$\>\ cat\ hola\ \$\>\ cat\ Docs/hey
run_test cd\ ../..\ \&\&\ pwd\ \&\&\ pwd
run_test \(cd\ ../..\ \&\&\ pwd\)\ \&\&\ pwd
run_test ls\ -z\ \|\|\ cd\ ../../..\&\&pwd\ \$\>\ pwd
run_test ls\ -z\ \|\|\ \(cd\ ../../..\&\&pwd\)\ \$\>\ pwd
run_test ls\ \*
run_test ls\ \*.\*
run_test ls\ \*.hola
run_test cat\ M\*le
run_test cat\ M\*ee
run_test cat\ Make\*file

echo "=== Test Results ==="
echo "Total tests: $test_count"
echo "Passed: $pass_count"
echo "Failed: $fail_count"
if [ $test_count -gt 0 ]; then
    score=$((pass_count * 100 / test_count))
    echo "Score: $score%"
fi
