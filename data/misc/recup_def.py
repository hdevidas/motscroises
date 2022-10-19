#!/usr/bin/python

fichierMot = open("data/word_database.txt")
FichierDef = open("data/clues.tsv")

fichierOut = open("data/word_with_def.txt", "a")

lignesWords = fichierMot.readlines()
lignesClues = FichierDef.readlines()


for mot in lignesWords: #parcours chacun des mots du fichier mot
    fichierOut.write(mot) #on l'écrit dans le fichier de sortie
    for clues in lignesClues: #parcourt tous les indices
      words = clues.split()
      try:
        if(words[2] + "\n" == mot):
          for i in range(3,len(words)):
            fichierOut.write(words[i] + " ")
          fichierOut.write("\n \n")
          break
      except IndexError:
          a = 5 #ne fait rien ...


fichierMot.close()
FichierDef.close()
fichierOut.close()