# age_of_heros
An Unreal Engine RTS game inspired by Age of Empires 2, developed in C++. 
>[!NOTE]
>This code is still in its early stages
__________________________________________________________________________________________________________

### Design Highlights

In designing the codebase, the primary focus was on clarity, scalability, and performance optimization for an RTS game environment. Key features include:

- **Custom Collision System and Grid System**: Implemented to enhance performance and streamline interactions within the game environment.
- **Objects Manager**: Employed for efficient resource management, ensuring smoother gameplay and optimized performance.
- **Optimized Algorithms**: Tailored algorithms to meet the specific needs of the game and its mechanics, enhancing overall performance and gameplay experience.
- **Modular Design for Flexibility**: Utilized a modular approach, separating UI/UX and gameplay design elements to facilitate easy modification and expansion.
- **Pathfinding Optimization**: Leveraged Theta* algorithm for smooth any-angle pathing and developed custom post-process algorithms for seamless agent movement. Additionally, implemented collision avoidance mechanisms to enable units to navigate around each other efficiently.
__________________________________________________________________________________________________________
### Project Overview

The real-time strategy (RTS) genre has always fascinated me. I embarked on this project initially as a journey to deepen my understanding of C++ and game programming as a whole. Throughout development, I've strived to uphold high standards in each aspect while crafting a captivating RTS experience inspired by Age of Empires 2.

The primary goal of this project is to create a multiplayer 1v1 RTS game, that pays homage to the timeless appeal of Age of Empires 2. However, I aim to inject some extra flair by introducing innovative gameplay mechanics. Instead of traditional civilizations, players will have the opportunity to command heroes from the Age of Empires 2 community. including their favorite players and casters :).
__________________________________________________________________________________________________________
### Development Progress

The development of this project is structured into three distinct phases, each aimed at achieving specific milestones and enhancing gameplay features:

1. **Phase One: Establishing the Foundation (Current Focus)**
   - The primary objective of the first phase is to establish a robust base code for the game, encompassing fundamental functionality essential for any RTS game.
   - During this phase, emphasis is placed on laying the groundwork for future expansion while exploring various design approaches to determine the project's overarching direction.

2. **Phase Two: Building a Playable singleplay Experience**
   - In the second phase, the focus shifts towards transforming the project into a playable 1v1 RTS game.
   - Players will have the opportunity to build and manage armies and economies, while also contending with simple AI opponents.
   - The goal is to deliver an early access version of the game that offers a refined and enjoyable gameplay experience.

3. **Phase Three: Introducing Hero System and Network Support**
   - The final phase centers on introducing advanced features such as the hero system and network support.
   - Players will be able to command heroes from the Age of Empires 2 community, adding depth and variety to gameplay.
   - Network support will enable players to challenge each other in thrilling 1v1 battles.
__________________________________________________________________________________________________________
### Here's some highlights :)
#### Unit avoidance.
![multi agents avoidance](/showcases/avoidance.gif)  
#### Villagers harvesting resources
![villagers harvesting resources](/showcases/harvesting.gif)
#### Path finding (Theta* + post process)
![pathfinding 1](/showcases/pathing2.gif)

![Pathfinding 2](/showcases/pathing1.gif)
