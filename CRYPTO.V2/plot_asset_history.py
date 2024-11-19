# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    plot_asset_history.py                              :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/16 19:13:44 by dinguyen          #+#    #+#              #
#    Updated: 2024/11/17 07:00:50 by dinguyen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import pandas as pd
import matplotlib.pyplot as plt

def plot_asset_history(csv_file):
	# Charger les données du fichier CSV
	data = pd.read_csv(csv_file)

	# Configuration du graphique
	plt.figure(figsize=(10, 6))

	# Ajouter les courbes
	plt.plot(data['Date'], data['Prix'], label='Prix', marker='o')
	plt.plot(data['Date'], data['Quantité'], label='Quantité', marker='x')
	plt.plot(data['Date'], data['Difference depuis achat'], label='Différence depuis achat', linestyle='--')
	plt.plot(data['Date'], data['Pourcentage depuis achat'], label='Pourcentage depuis achat', linestyle=':')

	# Ajouter des titres et légendes
	plt.title('Historique de l\'actif')
	plt.xlabel('Date')
	plt.ylabel('Valeur')
	plt.legend()
	plt.xticks(rotation=45)  # Rotation des dates pour les rendre lisibles
	plt.tight_layout()

	# Afficher le graphique
	plt.savefig('graphique_actif.png')
	plt.show()

# Exemple d'utilisation
if __name__ == "__main__":
	fichier_csv = input("Entrez le nom du fichier CSV exporté : ")
	plot_asset_history(fichier_csv)
