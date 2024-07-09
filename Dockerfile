FROM debian:buster
ENV VERSION 2.33
ENV DEBIAN_FRONTEND=noninteractive

# Mettre à jour et installer les paquets nécessaires
RUN apt-get update && apt-get install -y \
    build-essential \
    iproute2 \
    iputils-arping \
    && rm -rf /var/lib/apt/lists/*

# Copier les fichiers du projet dans le répertoire de travail
COPY . /home/user/ft_malcolm

# Définir le répertoire de travail
WORKDIR /home/user/ft_malcolm

# Compiler libft et ft_malcolm
RUN cd libft && make && cd .. && make

# Ouvrir un shell par défaut
CMD ["/bin/bash"]
