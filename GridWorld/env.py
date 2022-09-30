from os import terminal_size
import matplotlib.pyplot as plt
import pygame
import terminado

class GridWorld:
    def __init__(self, m, n, portals = {}):
        self.m, self.n = m, n

        pygame.init()
        global CELL_WIDTH, SCREEN, SCREEN_WIDTH, SCREEN_HEIGHT, CLOCK, AGENT_IMG, ENTRY_WORMHOLE_IMG, EXIT_WORMHOLE_IMG, EARTH_IMG
        CELL_WIDTH = 60
        SCREEN_WIDTH = CELL_WIDTH * m
        SCREEN_HEIGHT = CELL_WIDTH * n
        SCREEN = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
        CLOCK = pygame.time.Clock()
        AGENT_IMG = pygame.image.load('./imgs/UFO.png')
        AGENT_IMG = pygame.transform.scale(AGENT_IMG, (CELL_WIDTH / 1.5, CELL_WIDTH / 1.5))
        ENTRY_WORMHOLE_IMG = pygame.image.load('./imgs/entry_wormhole.png')
        ENTRY_WORMHOLE_IMG = pygame.transform.scale(ENTRY_WORMHOLE_IMG, (CELL_WIDTH / 1.5, CELL_WIDTH / 1.5))
        EXIT_WORMHOLE_IMG = pygame.image.load('./imgs/exit_wormhole.png')
        EXIT_WORMHOLE_IMG = pygame.transform.scale(EXIT_WORMHOLE_IMG, (CELL_WIDTH / 1.5, CELL_WIDTH / 1.5))
        EARTH_IMG = pygame.image.load('./imgs/earth.png')
        EARTH_IMG = pygame.transform.scale(EARTH_IMG, (CELL_WIDTH / 1.5, CELL_WIDTH / 1.5))

        self.portals = portals
        assert not (self.m - 1, self.n - 1) in self.portals.keys() # Make sure there is no entry for a portal in the terminal position

        # Actions
        self.transition = {
            'U' : (-1, 0),
            'D' : (1, 0),
            'L' : (0, -1),
            'R' : (0, 1)
        }

        self.agent_pos = (0, 0)

    def init_state(self):
        return (0, 0)

    def action_space(self):
        return list(self.transition.keys())

    def step(self, action):
        """
        Returns the new state, the reward and if the state is terminal
        Typically there's also a debug parameter return also
        """
        # Move
        self.agent_pos = ( 
            self.agent_pos[0] + self.transition[action][0], 
            self.agent_pos[1] + self.transition[action][1]
        )
        
        # Make sure we are within the boundaries
        self.agent_pos = (
            max(self.agent_pos[0], 0),
            max(self.agent_pos[1], 0)
        )

        self.agent_pos = (
            min(self.agent_pos[0], self.m - 1),
            min(self.agent_pos[1], self.n - 1)
        )

        self.agent_pos = self.portals.get(self.agent_pos, self.agent_pos)

        state_is_terminal = self.agent_pos[0] == self.m - 1 and self.agent_pos[1] == self.n - 1

        reward = 0 if state_is_terminal else -1

        return self.agent_pos, reward, state_is_terminal
    
    def render(self):
        CLOCK.tick(3)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                from sys import exit
                exit()

        # Fill the background with white
        SCREEN.fill((255, 255, 255))

        # Draw grid
        for x in range(0, SCREEN_WIDTH, CELL_WIDTH):
            for y in range(0, SCREEN_HEIGHT, CELL_WIDTH):
                rect = pygame.Rect(x, y, CELL_WIDTH, CELL_WIDTH)
                pygame.draw.rect(SCREEN, (0, 0, 0), rect, 1)

        # Draw EARTH_IMG
        SCREEN.blit(EARTH_IMG, ((self.m - 1) * CELL_WIDTH + CELL_WIDTH / 6, (self.n - 1) * CELL_WIDTH + CELL_WIDTH / 6))

        # Draw wormholes
        for e, l in self.portals.items():
            SCREEN.blit(ENTRY_WORMHOLE_IMG, (e[0] * CELL_WIDTH + CELL_WIDTH / 6, e[1] * CELL_WIDTH + CELL_WIDTH / 6))
            SCREEN.blit(EXIT_WORMHOLE_IMG, (l[0] * CELL_WIDTH + CELL_WIDTH / 6, l[1] * CELL_WIDTH + CELL_WIDTH / 6))

        # Draw agent
        SCREEN.blit(AGENT_IMG, (self.agent_pos[0] * CELL_WIDTH + CELL_WIDTH / 6, self.agent_pos[1] * CELL_WIDTH + CELL_WIDTH / 6))

        # Flip the display
        pygame.display.flip()

    def reset(self):
        self.agent_pos = (0, 0)

    def close(self):
        pygame.quit()

if __name__ == "__main__":
    from random import choice
    
    portals = {
        (1,1): (4,4),
        (4,6): (7,9)
    }

    env = GridWorld(10, 10, portals)

    games = 5
    total_rewards = [0] * games

    for g in range(games):
        print(f"Game {g+1} starting...")
        done = False
        ret = 0
        
        env.reset()

        while not done:
            action = choice(env.action_space)

            obs, reward, done = env.step(action)

            ret += reward
    
            env.render()

            #obs = obs_
        total_rewards[g] = ret
        print(f"Game {g+1} done...")

    env.close()
    #plt.plot(total_rewards)
    #plt.show()