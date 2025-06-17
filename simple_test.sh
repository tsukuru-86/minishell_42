#!/bin/bash

# Simple test script for basic commands
MINISHELL="./minishell"

echo "Testing basic syntax errors..."

# Test commands that should fail
echo "Test 1: | (pipe only)"
echo "|" | $MINISHELL
echo "Exit code: $?"
echo ""

echo "Test 2: | echo oi (pipe with command)"
echo "| echo oi" | $MINISHELL
echo "Exit code: $?"
echo ""

echo "Test 3: > (redirect only)"
echo ">" | $MINISHELL
echo "Exit code: $?"
echo ""

echo "Test 4: << (heredoc only)"
echo "<<" | $MINISHELL
echo "Exit code: $?"
echo ""

echo "Test 5: Simple echo"
echo "echo hello world" | $MINISHELL
echo "Exit code: $?"
echo ""

echo "Test 6: Echo with variables"
echo "echo \$USER" | $MINISHELL
echo "Exit code: $?"
echo ""
