TMUX
===
> last updated on 20181026
> 
# Concept
Server > Session > Window > pane
- Server
First run `tmux` will activate a server automatically, which will manage all sessions.
- Session
A terminal can open a tmux. There's a session number on the left-bottom side.
- Window
A session can include several windows, so if there's a running window, you can use another use to run other projects.
- pane
A window can contain several panes.
# Commands
- activate tmux and start a session
```shell
tmux
```
type `Ctrl+b` before using any shortcut
- **Pane**
`%` **:** divide a pane horizontally
`"` **:** divide a pane vertically
`directions` **:** switch between panes
`x` **:** close current pane

- **Window**
`c` **:** new a window
`&` **:** close current window
`p` **:** switch to previous window
`n` **:** switch to next window

- **Session** *(`$` : is command not shortcut)*
`$ tmux` **:** new a session
`$ tmux ls` **:** list all session
`$ tmux attach -t 0` **:** enter session 0
`$ tmux kill-session -t 0` **:** close session 0
`d` **:** detach current session (can come back w/ `tmux attach`)
`s` **:** switch between sessions
