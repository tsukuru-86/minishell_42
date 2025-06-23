#!/bin/bash

# Here Document test script
MINISHELL="./minishell"

echo "Testing Here Document functionality..."

echo "Test 1: << end cat -e"
echo "This tests the most important case from main.vala"
{
    echo "<< end cat -e"
    echo "simple"
    echo "test"
    echo "end"
} | $MINISHELL
echo "Exit code: $?"
echo ""

echo "Test 2: << EOF cat -e"
{
    echo "<< EOF cat -e"
    echo "\$USER"
    echo "EOF"  
} | $MINISHELL
echo "Exit code: $?"
echo ""

echo "Test 3: cat << here -e"
{
    echo "cat << here -e"
    echo "hello"
    echo "here"
} | $MINISHELL
echo "Exit code: $?"
echo ""

echo "Test 4: << AH cat -e | grep simple"
{
    echo "<< AH cat -e | grep -o 'simple'"
    echo "simple"
    echo "test"
    echo "end"
    echo "AH"
} | $MINISHELL
echo "Exit code: $?"
echo ""
