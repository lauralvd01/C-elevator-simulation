# Projet_SimulationAscenseur
Projet de semaine de dep - 24/06/2022 - Laura Levraud

**Version animée**

Le programme simule le comportement d'un ascenceur.
Toutes les minutes, de nouvelles personnes arrivent.
le scenario est intégré dans le main. Plusieurs personnes
peuvent arriver en même temps. Elles attendent le moment
où l'ascenceur est arrivé à leur étage.

Saisir manuellement l'étage à laquel vous souhaitez que l'ascenseur s'arrête

Le programme réalise les étapes suivantes par cycle de temps:
- il vérifie l'arrivée de nouvelles personnes
- il attend la saisie au clavier du numéro d'étage
- avant de partir, il fait rentrer les personnes dans l'ascenceur
- il ammène l ascenceur à l étage sélectionné (pas d'arrêt intermédiaire)
- une fois arrivé à l'étage voulu, il fait descendre les personnes concernées
- puis il fait rentrer les nouveaux arrivants

Le programme attend une nouvelle saisie de l'étage.
- la touche i permet de voir la liste des personnes présentes dans l'immeuble)
- la touche q permet de quitter la simulation)

**A améliorer:**

- rendre automatique l'automate en prenant en compte l'heure d'arrivée des personnes FIFO
- gérer les arrêts intermédiaires en fonction des personnes dans l'ascenceur
- depiler la liste une fois que la personne est sortie de l'ascenceur (chrono suivant)
- etc
