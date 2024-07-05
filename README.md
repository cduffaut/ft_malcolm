# Faire marcher tout ce code:
docker build -t ft_malcolm .

docker run --rm -it --privileged --network malcolm-net --name malcolm ft_malcolm

# reseau pour lier les deux dockers:
docker network create malcolm-net

# le test:
docker build -t ft_malcolm_test -f Dockerfile.test .

docker run --rm -it --network malcolm-net --name malcolm_test ft_malcolm_test

# supprimer les dockers en cours:
docker stop $(docker ps -a -q)
docker rm $(docker ps -a -q)

# sortir les IPs des containers:
docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' 4fe44bcf455d