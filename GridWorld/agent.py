from random import choice
from math import inf
import numpy as np

class Agent:
    """ Q-Learning agent"""
    def __init__(self, env, alpha=0.97, gamma = 0.97, epsilon=0.8):
        self.env = env

        self.state = env.init_state()
        self.last_action = None
        self.ret = [0]

        self.alpha = alpha
        self.gamma = gamma
        self.epsilon = epsilon

        self.qtable = {}
        for i in range(self.env.m):
            for j in range(self.env.n):
                self.qtable[(i,j)] = {'U':0, 'D':0, 'R':0, 'L':0}

    def get_action(self):
        rand = np.random.rand()
        if rand <= self.epsilon: # Take Random Action
            self.last_action = choice(self.env.action_space())
        else: # Take Best Estimated Action
            best_action = []
            best_action_qvalue = -inf
            for action in self.qtable[self.state]:
                if self.qtable[self.state][action] > best_action_qvalue:
                    best_action = [action]
                    best_action_qvalue = self.qtable[self.state][action]
                elif self.qtable[self.state][action] == best_action_qvalue:
                    best_action.append(action)
            self.last_action = choice(best_action)
        
        self.epsilon *= .98 # Reduce epsilon to 98% of its value, so random actions are taken less
        return self.last_action

    def observation(self, new_state, reward, done):
        # Get highest qvalue in the new state
        best_action_qvalue = -inf
        for action in self.qtable[new_state]:
            best_action_qvalue = max(best_action_qvalue, self.qtable[new_state][action])

        # Update qtable based on the rule
        self.qtable[self.state][self.last_action] += self.alpha * (reward + self.gamma * best_action_qvalue - self.qtable[self.state][self.last_action])

        # Update the state
        self.state = new_state

        self.ret[-1] += reward
        if done: self.ret.append(0)

    def stats(self):
        return self.ret