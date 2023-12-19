# 👨‍💻 Contribution au Projet PaxOS-8
**Chaque nouvelle Pull Request (PR) doit adhérer aux règles suivantes pour
être acceptée** :
- **Conventions de Codage** : Respectez les conventions de codage énoncées ci-dessous.


- **Absence de Warnings** : Assurez-vous que le code ajouté ne génère
pas de warnings lors de la compilation. Traitez tout avertissement émis par
le compilateur avant de soumettre la PR.

## ❓ Pourquoi des bonnes pratiques de codage en C++ ?

Le langage C++ est reconnu pour sa richesse et sa puissance,
mais cette complexité peut parfois rendre la collaboration difficile.
L'utilisation de bonnes pratiques de codage en C++ offre de nombreux avantages,
notamment :

- **Cohérence** : en adoptant des conventions de codage cohérentes, nous facilitons la lecture du
code pour l'ensemble de l'équipe de développement.


- **Maintenabilité** : des pratiques de codage bien établies rendent le code plus facile à comprendre,
à maintenir et à améliorer au fil du temps.


- **Sécurité** : en suivant des bonnes pratiques, nous contribuons à la réduction des erreurs
et des vulnérabilités potentielles dans notre code.


- **Performance** : Certaines approches de codage peuvent influencer de
manière favorable les performances du programme final.
Il est essentiel de garder à l'esprit que le code sera exécuté sur un _ESP32_.

## Nommage des variables
- **Nommer les variables de manière explicite** et :
- utilisez `pMaVariable` pour indiquer que c'est un pointeur.
- utilisez `sMaVariableStatique` pour indiquer que c'est une variable statique.
- utilisez `spMonPointeurStatique` pour indiquer que c'est un pointeur statique.
```c++
int* pMaVariable;
static sMaVariableStatique = 42;
static int* spMonPointeurStatique;
```
## Classes
- Commencez le nom des classes par un "**C**" majuscule.
- Commencez le nom de la variable par un `m_` pour indiquer que cette
variable est membre.
- ⚠️ **N'oubliez pas** : par défaut dans une structure, tout est en **privé**.
```c++
class CApp {
private:
    int m_id = 0;
    int* m_pId;
    static int m_sCount = 42;
    static int* m_spData;
};
```

## Structures
- Commencez le nom des structures par un "**S**" majuscule.
- Utilisez les structures uniquement si elles ne contiennent pas de méthodes.
- ⚠️ **N'oubliez pas** : par défaut dans une structure, tout est en **publique**.

## Enumérations
- Commencez le nom des énumérations par un "**E**" une majuscule.
```c++
enum EColor {
    Red,
    Green,
    Blue
};
```
## Utilisation de pointeurs
- **Évitez les raw pointers** :
- Utilisez des **smart pointers** plutôt que des raw pointers, sauf dans des cas extrêmes.
```c++
#include <memory>

std::unique_ptr<int> myUniquePtr = std::make_unique<int>(42);
std::shared_ptr<int> mySharedPtr = std::make_shared<int>(42);
```

## Utiliser les références à la place des pointeurs quand c'est possible
Lorsque vous avez la possibilité de choisir entre les références et les
pointeurs, optez pour les références. Les références offrent une syntaxe plus
claire et réduisent le risque d'erreurs liées à la gestion de la mémoire.
```c++
#include <iostream>
#include <string>

// Fonction utilisant une référence
void displayName(const std::string& value) {
    std::cout << "Hello world, " << value << " !" << std::endl;
}

int main() {
    // Variable entière
    std::string name = "Paxo-OS 8";

    // Appel de la fonction en passant la variable par référence
    displayName(name);

    return 0;
}
```

## Constness
- **Utilisez** le mot-clé `const` pour déclarer des variables qui ne doivent
pas être modifiées après leur initialisation.
```c++
const int constantValue = 42;
```
- Utilisez `const` pour indiquer si une méthode modifie
ou non l'état de l'objet. Les méthodes `const` ne modifient pas l'état de
l'objet et devraient être marquées comme telles.
```c++
class CMyClass {
public:
    int getValue() const        // Méthode constante
    {
        return m_id;
    }
    void setValue(int newId)    // Méthode non-constante
    {
        m_id = newId;
    }
    
private:
    int m_id = 0;
};
```
- Pointeurs const et objets const
```c++
const int* pImmutableValue;  // Pointeur vers une valeur immuable
int const* pAlsoImmutable;   // Équivalent

int const value = 42;        // Objet immuable
```
# Macros et Constantes
- Préférez `constexpr` aux **macros** (`#define`) :
-  Utilisez `constexpr` pour définir des constantes au
lieu de macros chaque fois que possible.
- **Les constantes déclarées avec `constexpr` sont plus
sûres en termes de portée et de type.**
```c++
// Utilisation de constexpr
constexpr int maxItems = 100;

// Évitez d'utiliser #define pour définir des constantes
#define MAX_ITEMS 100
```
- **Évitez les macros pour les fonctions** :
- Utilisez plutôt des fonctions en ligne (`inline`) ou des fonctions normales,
car elles offrent des avantages en termes de sécurité et de débogage.
```c++
#define SQUARE(x) ((x) * (x))

// Utilisez une fonction en ligne ou normale
inline int square(int x) {
    return x * x;
}
```

- **Utilisez `constexpr` pour les fonctions évaluables à la compilation :**
- Déclarez vos fonctions comme `constexpr` lorsque cela est possible.
Cela permet une évaluation à la compilation lorsque les arguments sont
des constantes connues à la compilation.
```c++
// Utilisation de constexpr pour une fonction non membre
constexpr int multiply(int a, int b) {
    return a * b;
}

// Exemple d'utilisation à la compilation
constexpr int result = multiply(3, 4);  // Évalué à la compilation
```

# Copies
- **Minimisez les copies inutiles dans les méthodes** :
- Évitez les copies inutiles d'objets en passant les
arguments par référence ou par référence constante plutôt que par valeur.
Les copies peuvent entraîner une surcharge inutile sur le système et impacter
les performances.
```c++
class CDataProcessor {
public:
    // Utilisation de référence constante pour minimiser les copies
    void processData(const std::vector<int>& data) {
        // Traitement des données ici
    }
};

```

# Utilisation Minimale de la Heap pour l'embarqué
- **Privilégiez la Stack** :
- Favorisez l'utilisation de la stack autant que possible pour allouer
des variables locales et temporaires.
La stack est plus rapide à accéder et libère automatiquement l'espace
lorsqu'une fonction se termine.
```c++
void exampleFunction() {
    int localVar;  // Allocation sur la stack
    // ...
}
```
- **Limitation des Allocations Dynamiques** :
- Limitez l'utilisation d'allocations dynamiques sur la heap.
Les allocations sur la heap peuvent entraîner une fragmentation de la
mémoire et sont généralement plus lentes que les allocations sur la stack.
```c++
// Éviter autant que possible les allocations sur la heap
int* dynamicValue = new int;  // À utiliser avec précaution
```

# Ressources pour s'améliorer en C++

**The Cherno** - _C++_ : https://www.youtube.com/watch?v=18c3MTX0PK0&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb