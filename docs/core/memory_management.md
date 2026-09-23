# This documents explains how memory is manages for the main classes of the Engine
The engine cor exposes 5 class types:
- **Aktor**: This are the caracters that interact in a *Scene* and they do *Behaviours*.
- **Behaviour**: This are the actions that an *Aktor* does when in play.
- **Scene**: This is a stage where *Aktors* *Behave* and live
- **Direktor**: This static class coordinates *Scenes* and *Actors* that are not associated with a *Scene*
- **Maestro**: This static class tells to *Aktos* what to do and asignes them *Behaviours*

## Relationships
| Class       | Creator   | Owner  | Destroyer | Updated   | Called by  |
| ----------- | --------- | ------ | --------- | --------- | ---------- |
| [A]ktor     | D         | D      | D         | S         | S,A,B,User |
| [B]ehaviour | M->A      | A      | A         | A         | User       |
| [S]cene     | D         | D      | D         | D         | A,B,User   |
| [D]irektor  | None      | None   | User      | User      | Global     |
| [M]aestro   | None      | None   | User      | None      | A,User     |

## Legenda
| KeyWord    | Meaning                                                                                                        |
| ---------- | -------------------------------------------------------------------------------------------------------------- |
| [KeyWord]  | The value under the keyword columb at that line                                                                |
| [Class]    | This rappresents the value in the Class columb                                                                 |
| [Ref]      | This rappresents the values of intersection between Class and '[KeyWord]' in the previous table                |
| Creator    | Who to call when making a new of [Class] this ensures that the class is configured corectly                    |
| Owner      | After the creation of an objekt of type [Class]: [Ref] owns it's life time                                     |
| Destroyer  | if [Class] of Owner and Destroyer are the same deallocation appens when the owner is deallocated               |
| Updated    | Who starts, updates... this [Class] note that user can only call on Director                                   |
| Called by  | Who knows and can modify that [Class], owners always know owned objekts                                        |
| User       | The programmer of the code is resposible for [Keyword] or can interact with is [Class]                         |
| M->A       | This rappresent that objekt if type Behaviour are instanciated by Maestro via the Actor to witch they make act |
| None       | This signals that the class is static and no one instanciates it or owns it, or it cannot be called            |
| Global     | This Signals that every one kwnows about this [Class] and can modify it                                        |
