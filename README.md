# Faire marcher tout ce code:
docker build -t ft_malcolm .

# Reseau pour lier les deux dockers:
docker network create malcolm-net

# Récupérer les bon IPs et MAC adresses pour le test:

- Lancer le programme ft_malcolm avec des fake IP et MAC
- Faire un arping
- Et regarder les résultats du arping pour déduire un IP et adresse MAC valable ✅

# Terminal 1:

docker run --rm -it --cap-add=ALL --network malcolm-net --name malcolm ft_malcolm /bin/bash

./ft_malcolm 172.18.0.2 aA:bb:cc:dd:EE:ff 172.18.0.3 02:42:ac:12:00:03

# Terminal 2:
docker run --rm -it --network malcolm-net --name requester ft_malcolm /bin/bash

apt-get update && apt-get install -y iputils-arping
arping -I eth0 172.18.0.2
