import random
class MyAgent20876076(rl_agent.AbstractAgent):

  totalRock = 0
  totalPaper = 0    #Total number of R/P/S used
  totalScissor = 0
  tenRock = 0
  tenPaper = 0      #Total number of R/P/S used in last 10 moves
  tenScissor = 0
  oppLastWin = False    #if opponent won last move
  numOppWinChange = 0   #number of times opponent changes move after they win
  numOppWinStay = 0     #number of times opponent doesn't changs moves after they win
  numOppLoseChange = 0    #number of times opponent changes move after they lose
  numOppLoseStay = 0      #number of times opponent doesn't change moves after they lose
  difference = 0
  numRR = 0
  numRS = 0
  numRP = 0
  numPR = 0       #number of times they switch from one move to another
  numPS = 0
  numPP = 0
  numSR = 0
  numSS = 0
  numSP = 0
  prevRespLst = [-1, -1, -1, -1, -1, -1]
  prevRespLstRslt = [0, 0, 0, 0, 0, 0]
  randWeight = 5
  sumWeight = 5
  tenWeight = 30
  lastMoveWeight = 70     #weights of each factor
  winWeight = 40
  loseWeight = 10


  def __init__(self, num_actions, name="20876076_agent"):
    assert num_actions > 0
    self._num_actions = num_actions  # 3

  def restart(self):    # resets each value after each set of episodes
    self.totalRock = 0
    self.totalPaper = 0
    self.totalScissor = 0
    self.tenRock = 0
    self.tenPaper = 0
    self.tenScissor = 0
    self.numOppWinChange = 0
    self.numOppWinStay = 0
    self.numOppLoseChange = 0
    self.numOppLoseStay = 0
    self.difference = 0
    self.numRR = 0
    self.numRS = 0
    self.numRP = 0
    self.numPR = 0
    self.numPS = 0
    self.numPP = 0
    self.numSR = 0
    self.numSS = 0
    self.numSP = 0
    self.prevRespLst = [-1, -1, -1, -1, -1, -1]
    self.prevRespLstRslt = [0, 0, 0, 0, 0, 0]
    self.randWeight = 5
    self.sumWeight = 5
    self.tenWeight = 30
    self.lastMoveWeight = 70
    self.winWeight = 40
    self.loseWeight = 10

  def returnMax(self, v1, v2, v3):    #returns best move based on input
    if v1 > v3:
      if v1 > v2:
        return 1
      else:
        return 2
    else:
      if v3 > v2:
        return 0
      else:
        return 2

    return 0

  def adjustNextPlay(self, lastPlay, currPlay): #adjusts opponents move trends
    R = 0
    P = 1
    S = 2

    if lastPlay == R:
      if currPlay == R:
        self.numRR += 1
      elif currPlay == P:
        self.numRP += 1
      else:
        self.numRS += 1
    elif lastPlay == P:
      if currPlay == R:
        self.numPR += 1
      elif currPlay == P:
        self.numPP += 1
      else:
        self.numPS += 1
    else:
      if currPlay == R:
        self.numSR += 1
      elif currPlay == P:
        self.numSP += 1
      else:
        self.numSS += 1

  def checkOptResp(self, oppMove):    #Checks what the opponent played and which factors played correctly 
    R = 0
    P = 1
    S = 2
    for i in range(6):
      if oppMove == R:
        if self.prevRespLst[i] == R:
          self.prevRespLstRslt[i] = 0
        elif self.prevRespLst[i] == P:
          self.prevRespLstRslt[i] = 1
        else:
          self.prevRespLstRslt[i] = -1
      elif oppMove == P:
        if self.prevRespLst[i] == R:
          self.prevRespLstRslt[i] = -1
        elif self.prevRespLst[i] == P:
          self.prevRespLstRslt[i] = 0
        else:
          self.prevRespLstRslt[i] = 1
      else:
        if self.prevRespLst[i] == R:
          self.prevRespLstRslt[i] = 1
        elif self.prevRespLst[i] == P:
          self.prevRespLstRslt[i] = -1
        else:
          self.prevRespLstRslt[i] = 0


  def adjustWeight(self):   #adjusts the weights of the factors

    if self.prevRespLstRslt[0] == 1:
      self.randWeight += 1
    elif self.randWeight >= 5 and self.prevRespLstRslt[0] == -1:  #random factor
      self.randWeight -= 1

    if self.prevRespLstRslt[1] == 1:
      self.sumWeight += 1
    elif self.sumWeight >= 5 and self.prevRespLstRslt[1] == -1: #sum total factor
      self.sumWeight -= 1

    if self.prevRespLstRslt[2] == 1:
      self.tenWeight += 1
    elif self.tenWeight >= 5 and self.prevRespLstRslt[2] == -1: #past 10 moves factor
      self.tenWeight -= 1

    if self.prevRespLstRslt[3] == 1:
      self.lastMoveWeight += 1
    elif self.lastMoveWeight >= 5 and self.prevRespLstRslt[3] == -1:  #last move weight
      self.lastMoveWeight -= 1

    if self.prevRespLstRslt[4] == 1:
      self.winWeight += 1
    elif self.winWeight >= 5 and self.prevRespLstRslt[4] == -1:   # if opponent has just won
      self.winWeight -= 1

    if self.prevRespLstRslt[5] == 1:
      self.loseWeight += 1
    elif self.loseWeight >= 5 and self.prevRespLstRslt[5] == -1:    #if opponent has just lost
      self.loseWeight -= 1

  def returnNextPlay(self, recent, oppWon = 2):   #returns next best move based on opponents previous trends

    R = 0
    P = 1
    S = 2

    if oppWon == 1 and (self.numOppWinStay - self.numOppWinChange > self.difference or self.numOppLoseStay - self.numOppLoseChange > self.difference):
      if recent == R:
        return P
      elif recent == P:
        return S
      else:
        return R
    elif oppWon == -1 and (self.numOppWinChange - self.numOppWinStay > self.difference or self.numOppLoseChange - self.numOppLoseStay > self.difference):
      if recent == R:
        if self.numRP > self.numRS:
          return S
        else:
          return R
      elif recent == P:
        if self.numPS > self.numPR:
          return R
        else:
          return P
      else:
        if self.numSP > self.numSR:
          return S
        else:
          return P
    else:
      if recent == R:
        if self.numRR > self.numRP:
          if self.numRR > self.numRS:
            return P
          else:
            return R
        else:
          if self.numRP > self.numRS:
            return S
          else:
            return R
      elif recent == P:
        if self.numPR > self.numPP:
          if self.numPR > self.numPS:
            return P
          else:
            return R
        else:
          if self.numPP > self.numPS:
            return S
          else:
            return R
      else:
        if self.numSR > self.numSP:
          if self.numSR > self.numSS:
            return P
          else:
            return R
        else:
          if self.numSP > self.numSS:
            return S
          else:
            return R

  def step(self, time_step, is_evaluation=False):
    # If it is the end of the episode, don't select an action.
    if time_step.last():
      return

    game, state = pyspiel.deserialize_game_and_state(time_step.observations["serialized_state"])

    action = random.randint(0,2)
    optionLst = [0, 1, 2, 0, 1, 2]
    randWeight = self.randWeight
    sumWeight = self.sumWeight
    tenWeight = self.tenWeight
    lastMoveWeight = self.lastMoveWeight
    winWeight = self.winWeight
    loseWeight = self.loseWeight

    if len(state.history()) > 0:
      self.adjustWeight()
      R = 0
      P = 1
      S = 2
      recent = state.history()[-1]
      optionLst[0] = action
      winStay = False
      winChange = False
      loseStay = False
      loseChange = False

      if len(state.history()) > 3:
        self.adjustNextPlay(state.history()[-3], recent)  #adjusts next move

      if len(state.history()) < 100:
        self.difference = 10
      elif len(state.history()) < 200:    #adjusts difference for determining how many times opponent has to win before its considered signifigant 
        self.difference = 15
      elif len(state.history()) < 500:
        self.difference = 20
      else:
        self.difference = 25

      if time_step.rewards[-1] == 1 and len(state.history()) > 3:

        if state.history()[-1] == state.history()[-3] and self.oppLastWin:
          self.numOppWinStay += 1
        else:
          self.numOppWinChange += 1

        if self.numOppWinStay - self.numOppWinChange > self.difference:
          winStay = True
        elif self.numOppWinChange - self.numOppWinStay > self.difference:
          winChange = True

      elif time_step.rewards[-1] == -1 and len(state.history()) > 3:
        if state.history()[-1] == state.history()[-3] and not self.oppLastWin:
          self.numOppLoseStay += 1
        else:
          self.numOppLoseChange += 1

        if self.numOppLoseStay - self.numOppLoseChange > self.difference:
          loseStay = True
        elif self.numOppLoseChange - self.numOppLoseStay > self.difference:
          loseChange = True

        optionLst[3] = self.returnNextPlay(recent, 2)

        if time_step.rewards[-1] == 1:
          optionLst[4] = self.returnNextPlay(recent, time_step.rewards[-1])   #sets weights for win factor and lose factor
          optionLst[5] = self.returnNextPlay(recent, 2)
        elif time_step.rewards[-1] == -1:
          optionLst[4] = self.returnNextPlay(recent, 2)
          optionLst[5] = self.returnNextPlay(recent, time_step.rewards[-1])
        else:
          optionLst[4] = self.returnNextPlay(recent, 2)
          optionLst[5] = self.returnNextPlay(recent, 2)

      if recent == R:
        self.totalRock += 1
        self.tenRock += 1
      elif recent == P:         #increases sum for each played move
        self.totalPaper += 1
        self.tenPaper += 1
      else:
        self.totalScissor += 1
        self.tenScissor += 1

      optionLst[1] = self.returnMax(self.totalRock, self.totalPaper, self.totalScissor)   #sets sum total weight factor

      if len(state.history()) >= 20:
        if state.history()[-19] == R:
          self.tenRock -= 1
        elif state.history()[-19] == P:
          self.tenPaper -= 1
        else:
          self.tenScissor -= 1

        optionLst[2] = self.returnMax(self.tenRock, self.tenPaper, self.tenScissor)   #sets past 10 moves weight factor


      if winStay or winChange:    #if opponent has won a 'difference' times and has won past game, set weight as follows
        winWeight = 70
        loseWeight = 0
        tenWeight = 15
        lastMoveWeight = 5

      if loseStay or loseChange:     #if opponent has lost a 'difference' times and has lost past game, set weight as follows
        loseWeight = 70
        WinWeight = 0
        tenWeight = 15
        lastMoveWeight = 5

      if self.tenRock > 6 or self.tenPaper > 6 or self.tenScissor > 6:    #if opponent has played same move > 7 times in the past 10 moves, adjust weight
        tenWeight = 70
        lastMoveWeight = 0
        if winWeight > loseWeight:
          winWeight = 30
          loseWeight = 0
        elif loseWeight > winWeight:
          winWeight = 0
          loseWeight = 30


      action = random.choices(optionLst, weights=(randWeight, sumWeight, tenWeight, lastMoveWeight, winWeight, loseWeight))[0]    #choose factor

      self.checkOptResp(state.history()[-1])
      self.prevRespLst = optionLst      #adjust weights based on previous moves (regret)
      self.adjustWeight()

      if time_step.rewards[-1] == 1:
        self.oppLastWin = True
      else:
        self.oppLastWin = False

    probs = np.ones(self._num_actions) / self._num_actions
    return rl_agent.StepOutput(action=action, probs=probs)