import json
import random
import sys

def error ( message:str ):
	print ( message )

words = dict ( )
try:
	with open ( "Data/words", 'r' ) as f:
		try:
			words = json.load ( f )
		except json.decoder.JSONDecodeError:
			error ( "Json load error" )
except FileNotFoundError:
	error ( "no words?" )

random.seed ( sys.argv[1] )

## pick the word
try:
	letter = random.choice ( list ( words.keys ( ) ) )
	word = random.choice ( words [ letter ] )
except IndexError:
	error ( 'no words in list' )

## suppongo sia  len 5
guess = sys.argv [ 2 ]

guess = guess.upper ( )

right = []
quasi = []
for index, char in enumerate ( word ):
	if ( char == guess[index] ):
		right.append ( index )
	for c in range (0,5):
		if ( char == guess [c] and c != index ):
			quasi.append ( c )

for i in range (0,5):
	if i in right:
		print ( 'G', end='' ) # giusto
	elif i in quasi:
		print ( 'Q', end='' ) # quasi
	else:
		print ( '0', end='' ) # sbaglaito
