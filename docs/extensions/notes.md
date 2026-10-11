### Phisicks system
The following are aviable when using the Phisicks system
| Method       | Arguments  | When it is called                                         |
| ------------ | ---------- | --------------------------------------------------------- |
| Fixed_Update | ```void``` | Every time the Phisicks system updates
| Collision       | ```std::shared_ptr<Objekt>``` | Every Fixed_Update between Collision_Enter and Collision_Exit
| Collision_Enter | ```std::shared_ptr<Objekt>``` | When the objekt enters in collision with another one
| Collision_Exit  | ```std::shared_ptr<Objekt>``` | When the objekt exits collision with another one
| Collision_Trigger       | ```std::shared_ptr<Objekt>``` | Every Fixed_Update between Collision_Trigger_Enter and Collision_Trigger_Exit
| Collision_Trigger_Enter | ```std::shared_ptr<Objekt>``` | When the object enters a trigger volume
| Collision_Trigger_Exit  | ```std::shared_ptr<Objekt>``` | When the object exits a trigger area 
( Note: the passed argument for the Collision methods is the collided object )


