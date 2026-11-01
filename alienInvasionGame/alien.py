import pygame
from pygame.sprite import Sprite

class Alien(Sprite):
    """A class to represent a single alien in the field"""

    def __init__(self, game_instance):
        """Initialize the alien and set its starting position"""
        super().__init__()
        self.screen = game_instance.screen
        self.settings = game_instance.settings

        # Load alien image and set its rect attribute
        self.image = pygame.image.load("images/alien.png")
        self.rect = self.image.get_rect()

        # Start each new alien near the top of the screen
        self.rect.x = self.rect.width
        self.rect.y = self.rect.height

        # Store the alien's horizontal position
        self.x = float(self.rect.x)

    def update(self):
        """Move the alien"""
        self.x += (self.settings.alien_speed * self.settings.fleet_direction)
        self.rect.x = self.x

    def check_edges(self):
        """Return true if alien is at the edge of the screen"""
        screen_rect = self.screen.get_rect()
        if self.rect.right >= screen_rect.right or self.rect.left <= 0:
            return True