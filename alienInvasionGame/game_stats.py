class GameStats:
    """Track statistics for Alien Invasion."""

    def __init__(self, game_instance):
        """Initialize statistics for Alien Invasion."""
        self.settings = game_instance.settings
        self.reset_stats()

        # Check for existing high score from file
        with open('high_score_record.txt') as file:
            load_record = file.read()
        self.high_score = int(load_record.rstrip())

        # Start Alien Invasion in an inactive state
        self.game_active = False

    def reset_stats(self):
        """Initialize statistics that can change during the game."""
        self.ships_left = self.settings.ship_limit
        self.score = 0
        self.level = 1