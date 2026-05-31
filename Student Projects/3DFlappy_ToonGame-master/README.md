# Flappy Toon 3D

A simple 3D remake of the classic Flappy Bird game using **OpenGL** and **GLUT** in C++.
The project demonstrates core Computer Graphics concepts including lighting, textures, animation, camera movement, collision detection, and interactive controls.

---

## 🎮 Game Overview

In this game, the player controls a flying bird and tries to pass through moving pipes without crashing.

The project was developed as a final project for the **Computer Graphics** course and focuses on implementing real-time rendering techniques and interactive gameplay mechanics using OpenGL.

---

# ✨ Features

* ✅ 3D Environment Rendering
* ✅ Dynamic Lighting System
* ✅ Texture Mapping
* ✅ Camera Following & Camera Shake
* ✅ Collision Detection
* ✅ Keyboard & Mouse Controls
* ✅ Bird Animation
* ✅ Particle Explosion Effects
* ✅ Health System
* ✅ Score & High Score System
* ✅ HUD (Heads-Up Display)

---

# 🛠 Technologies Used

* **C++**
* **OpenGL**
* **GLUT**
* **stb_image** (for texture loading)

---

# 🎮 Controls

| Key / Mouse        | Action                    |
| ------------------ | ------------------------- |
| `SPACE`            | Bird Jump / Start / Retry |
| `W`                | Bird Jump                 |
| `UP ARROW`         | Bird Jump                 |
| `Left Mouse Click` | Bird Jump                 |
| `ESC`              | Exit Game                 |

---

# 🧠 Graphics Concepts Implemented

## 1. Lighting

The project uses OpenGL lighting to simulate realistic illumination effects.

Implemented features:

* Ambient Light
* Diffuse Light
* Specular Light
* Smooth Shading

Functions:

* `setupLighting()`

---

## 2. Texture Mapping

Textures are applied to:

* Bird
* Pipes
* Ground
* Sky

Functions:

* `loadTexture()`
* `drawBird()`
* `draw3DBox()`
* `drawSky()`

---

## 3. Camera System

A dynamic camera follows the bird smoothly and includes a shake effect during collisions.

Functions:

* `Camera::update()`
* `Camera::apply()`

---

## 4. Collision Detection

Collision detection is implemented between:

* Bird & Pipes
* Bird & Ground

Functions:

* `hitsPipe()`

---

## 5. Animation

Animations implemented:

* Bird wing flapping
* Bird tilt rotation
* Scrolling ground and sky
* Particle explosion effects

Functions:

* `BirdAnim::update()`
* `updateParticles()`
* `drawParticles()`

---

## 6. 3D Environment

The environment is fully rendered in 3D using textured boxes.

Includes:

* Ground
* Pipes
* Sky background

Functions:

* `draw3DBox()`
* `drawGround()`
* `drawPipes()`
* `drawSky()`

---

# 💥 Particle System

A particle system was implemented to create explosion effects when collisions occur.

Features:

* Random particle movement
* Gravity effect
* Lifetime fading
* Dynamic coloring

Functions:

* `burst()`
* `updateParticles()`
* `drawParticles()`

---

# ❤️ Health System

The game includes a health bar system:

* Collisions decrease health
* Health bar updates dynamically
* Game ends when health reaches zero

Functions:

* `drawHUD()`

---

# 🏆 Score System

* Score increases when the player successfully passes pipes.
* High score is saved during gameplay.

Variables:

* `score`
* `hiScore`

---

# 📂 Project Structure

| File / Section   | Purpose                        |
| ---------------- | ------------------------------ |
| Texture Loader   | Load image textures            |
| Camera System    | Camera follow & shake          |
| Lighting Setup   | OpenGL lighting                |
| Bird Animation   | Bird movement & wing animation |
| Collision System | Detect collisions              |
| HUD System       | Draw health and score          |
| Particle System  | Explosion effects              |
| Game Loop        | Update physics and rendering   |

---

# 🚀 How to Run

1. Open the project in **Visual Studio**
2. Make sure OpenGL & GLUT libraries are installed
3. Add texture images to the project folder:

   * `Bird.jpg`
   * `pipeBottom.jpg`
   * `Grass.jpg`
   * `photo0jpg.jpg`
4. Build and run the project

---

# 📸 Screenshots

Add your screenshots here:

<img width="1919" height="1004" alt="Screenshot 2026-05-15 203912" src="https://github.com/user-attachments/assets/c9a93e10-b7fa-449a-977c-11a36bdacd50" />
<img width="1919" height="1007" alt="Screenshot 2026-05-15 203928" src="https://github.com/user-attachments/assets/d050e876-5983-41eb-b2e9-43edbd8cb9db" />


---

# 📌 Learning Outcomes

This project helped in understanding:

* Real-time rendering
* OpenGL pipeline
* Transformations
* Texture mapping
* Lighting models
* Animation systems
* Interactive game loops
* Collision detection techniques

---

# 👨‍💻 Developer

Developed by: **Mohanad Mokhtar**
Faculty of Computers & Artificial Intelligence

---

# 📖 Conclusion

Flappy Toon 3D is a lightweight OpenGL game project designed to demonstrate important Computer Graphics concepts in a fun and interactive way. The project combines gameplay mechanics with real-time rendering techniques to create a complete 3D mini-game experience.
