import spur

shell = spur.SshShell(hostname="172.17.0.1", username="nano", password="wait9318")
with shell:
    result = shell.run(["echo", "-n", "hello"])
print(result.output) # prints hello
