# Engine Core Classes
This is an overview of the classes. For the specifics of every class read the appropriate document.

## Core Terms

- **[O]pera**: The spectacle the other classes are performing.
- **[A]ktor**: A character that lives in a Scene and performs Behaviours.
- **[B]ehaviour**: An action performed by an Aktor during a scene.
- **[S]cene**: The location where Aktors exist, interact, and perform Behaviours.
- **[D]irektor**: A class that manages Scenes and oversees scene flow.
- **[M]aestro**: A class that assigns Behaviours to Aktors when needed.

## Logical Structure

- There is only 1 Director and 1 Maestro per Opera.
- Director recruits Aktors and assignes them to Scenes.
- Behaviours can be ether active or inactive, all active behaviours will be performed during the scene.
- Aktors act only inside scenes and can move between them.
- Scenes contain Aktors and define the environment for their Behaviours.
- A single Scene can be marked as Active at any given time.
- Direktor controls which Scene is active and handles scene transitions.
- When an Aktor needs to do a Behaviour for the first time Maestro is resposible to teach him.
- When the scope of a scene ends, the next scene is played.
- An actor can be assigned to more than one scene.
- Director is resposble for all actors.
- Scenes can refence assigned actors but are not resposible for them.

## Glossary

- **Action**: An action is everything an aktor can do: jumping, drawing, taking a photo...
- **Perform**: Is the act of executing active behaviours.
- **Suspended**: When an actor is not on the Active Scene it is suspended and does nothing until it enters the Active Scene.
- **Retired**: When an actor is retired is permanently removed from the opera.
- **Scene Transition**: The moment when the Active Scene is changed.
- **Being Responsible**: When an Aktor A1 is responsible for another Aktor A2 the following happens:
       - if A1 is moved (goes to another scene / suspended) form a scene A2 follows.
       - if A1 is retired A2 is retired too.
       - if A2 is moved to another scene independently of A1 then A2 becomes free, and A1 is not responsible of A2 any more.
       - There cannot be a scenario when A1 is responsible of A2 and A2 is responsible for A1
       - If A2 is responsible for A3, A1 is also responsible for A3

## Example Hierarchy Graph
riscrivi il grafico con Directror ( actors then scens and actors with *An)
```                                                         
┌────────────────────────────────────────────────────────────────┐
│ Opera        ┌───┐           ┌───┐                             │
│              │ D │           │ M │                             │
│              └─┬─┘           └───┘                             │
│      ┌──────┬──┴─────────────┬──────┬─────────────┐            │
│    ┌─▼──┐ ┌─▼──┐           ┌─▼──┐ ┌─▼──┐        ┌─▼──┐         │
│    │ A1 │ │ S1 │           │*S2 │ │ S3 │        │ S4 │         │
│    └─┬──┘ └─┬──┘           └─┬──┘ └────┘        └─┬──┘         │
│    ┌─▼─┐    │         ┌──────┼────────────┐       ├──────┐     │
│    │ B │  ┌─▼──┐    ┌─▼──┐ ┌─▼──┐       ┌─▼──┐  ┌─▼──┐ ┌─▼──┐  │
│    └───┘  │ A2 │    │ A5 │ │ A3 │       │ A6 │  │ A4 │ │ A6 │  │
│           └─┬──┘    └─┬──┘ └─┬──┘       └─┬──┘  └─┬──┘ └─┬──┘  │
│    ┌─────┬──┴──┐    ┌─▼─┐    ├─────┐    ┌─▼─┐   ┌─▼─┐  ┌─▼─┐   │
│  ┌─▼─┐ ┌─▼─┐ ┌─▼─┐  │ B │  ┌─▼─┐ ┌─▼──┐ │ B │   │ B │  │ B │   │
│  │ B │ │ B │ │ B │  └───┘  │ B │ │ A4 │ └───┘   └───┘  └───┘   │
│  └───┘ └───┘ └───┘         └───┘ └─┬──┘                        │
│                                  ┌─▼─┐                         │
│                                  │ B │                         │
│                                  └───┘                         │
└────────────────────────────────────────────────────────────────┘
```

- A1 is idle and is managed by the Direktor.
- A2 has a role in S1 and performs three Behaviours.
- S2 contains 4 Aktors ( A5,A6,A3,A4 ).
- A3 is responsible for A4.
- S3 is empty and is being prepared.
- M remains behind the curtain as a silent instructor.
- D as the director oversees the Scenes flow.
- A4 and A6 as you can see both act on S2 and S4.
