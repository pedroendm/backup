from env import GridWorld    
from agent import Agent
import matplotlib.pyplot as plt

portals = {
    (1,1): (4,4),
    (4,6): (7,9)
}

env = GridWorld(10, 10, portals)

agent = Agent(env)

episodes = 500

for ep in range(episodes):
    print(f"Game {ep+1} starting...")
    done = False
        
    env.reset()

    while not done:
        if ep + 1 == episodes: # Just render last episode
            env.render()

        action = agent.get_action()

        new_state, reward, done = env.step(action)

        agent.observation(new_state, reward, done)
    
    print(f"Game {ep+1} done...")

env.close()

plt.plot(agent.stats())
plt.title('Agent\'s return through the episodes')
plt.xlabel('Episode')
plt.ylabel('Return')
plt.show()