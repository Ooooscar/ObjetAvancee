# Compilateur C++ ("CXX") :
CXX= g++

# Options ("flags") du compilateur C++ :
# --std=c++11 : Utilise le standard C++11
# -Wall : ("Warning all") Active tous les messages d'avertissement
# -MMD : Génère automatiquement les dépendances pour chaque fichier d'en-tête inclus
#        et les enregistrer en créant des fichiers .d
CXXFLAGS= --std=c++11 -Wall -MMD

# Options du PréProcesseur C ("CPP") :
# -I : Spécifie le chemin des fichiers d'en-tête ("header files", .h et .hpp) 
CPPFLAGS= -I $(CURDIR)/SFML-2.6.1/include

# Options du lien dynamique ("Linker (ld) Flags") :
# -L : Spécifie le chemin des bibliothèques
LDFLAGS= -L $(CURDIR)/SFML-2.6.1/lib

# Bibliothèques à lier ("Linker Libraries") :
# -l : Spécifie le nom d'une bibliothèque à lier
LDLIBS= -l sfml-system -l sfml-graphics -l sfml-window

# Commande de suppression des fichiers ("Remove") :
# -f : ("force") N'affiche pas de messages d’erreur si un fichier n’existe pas
#      et ignore les protections en écriture
RM= rm -f


# Liste des fichiers sources
SOURCES = ebaucheModel.cpp ebaucheVue.cpp
# Liste des fichers objets
OBJECTS = $(SOURCES:.cpp=.o)


# Cible par défaut de la commande "make" :
all: ebaucheVue

# Cibles "%.o" pour générer un fichier objet (.o) à partir de chaque fichier source (.cpp) :
# $< : Référence au premier fils de la règle (ici le nom du ficher .cpp)
# -o : ("output") On spécifie ensuite le nom du fichier de sortie
# $@ : Référence à la cible de la règle (ici le nom du ficher .o)
# -c : ("compile only") Génère un fichier .o au lieu d'un exécutable final
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@ -c

# Cibles pour générer les exécutables à partir des fichiers objets :
# $^ : Référence à tous les fils d'une règle
ebaucheModel: ebaucheModel.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)
ebaucheVue: ebaucheVue.o Niveau.o ebaucheModel.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)


# Cible "test" pour exécuter l'exécutable final :
test:
	./ebaucheVue

# Cible "clean" pour nettoyer les fichiers générés :
clean:
	$(RM) *.o *.d ebaucheVue ebaucheVue.exe a.out a.exe

# Inclure les fichiers de dépendances (.d) générés automatiquement pour chaque fichier objet (.o) :
-include $(OBJECTS:.o=.d)
