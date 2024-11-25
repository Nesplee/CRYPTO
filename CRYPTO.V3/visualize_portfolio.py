# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    visualize_portfolio.py                             :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/25 01:46:44 by dinguyen          #+#    #+#              #
#    Updated: 2024/11/25 05:21:38 by dinguyen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import json
import matplotlib.pyplot as plt
import pandas as pd

# Charger les données JSON
with open("MAIN.txt", "r") as file:
    portfolio_data = json.load(file)

def prepare_data(data):
    """Prépare les données et divise les actifs en Tableau A et Tableau B."""
    assets = data["assets"]
    line_data = []
    tableau_a = []  # Actifs avec prix min >= 1$
    tableau_b = []  # Actifs avec prix max <= 10$ et prix min < 1$

    for asset in assets:
        name = asset["name"]
        historique = asset["historique"]

        for entry in historique:
            line_data.append({
                "Nom": name,
                "Date": entry["date"],
                "Prix": entry["prix"]
            })

        min_price = min(h["prix"] for h in historique)
        max_price = max(h["prix"] for h in historique)

        if min_price >= 1:
            tableau_a.append({"Nom": name, "Prix Moyen": sum(h["prix"] for h in historique) / len(historique)})
        elif max_price <= 10:
            tableau_b.append({"Nom": name, "Prix Moyen": sum(h["prix"] for h in historique) / len(historique)})

    line_df = pd.DataFrame(line_data)
    line_df["Date"] = pd.to_datetime(line_df["Date"])
    line_df = line_df.sort_values(by="Date")

    return line_df, pd.DataFrame(tableau_a), pd.DataFrame(tableau_b)

def assign_colors(names):
    """Attribue une couleur unique pour chaque actif."""
    color_map = plt.cm.get_cmap("tab10", len(names))  # Palette de 10 couleurs répétées
    return {name: color_map(i) for i, name in enumerate(names)}

def visualize_portfolio(line_df, tableau_a, tableau_b):
    """Affiche les informations sous forme de graphiques."""
    fig, axs = plt.subplots(2, 2, figsize=(16, 12))
    fig.suptitle("Représentation Graphique du Portefeuille", fontsize=16)

    # Tableau A
    ax1 = axs[0, 0]
    filtered_a = line_df[line_df["Nom"].isin(tableau_a["Nom"])]
    colors_a = assign_colors(tableau_a["Nom"])
    for name, group in filtered_a.groupby("Nom"):
        ax1.plot(group["Date"], group["Prix"], label=name, color=colors_a[name])
    ax1.set_title("TABLEAU A : Actifs (Prix Min ≥ 1 $)")
    ax1.set_xlabel("Dates")
    ax1.set_ylabel("Prix ($)")
    ax1.legend(loc="best", fontsize="small")

    # Tableau B
    ax2 = axs[0, 1]
    filtered_b = line_df[line_df["Nom"].isin(tableau_b["Nom"])]
    colors_b = assign_colors(tableau_b["Nom"])
    for name, group in filtered_b.groupby("Nom"):
        ax2.plot(group["Date"], group["Prix"], label=name, color=colors_b[name])
    ax2.set_title("TABLEAU B : Actifs (Prix Max ≤ 10 $ et Min < 1 $)")
    ax2.set_xlabel("Dates")
    ax2.set_ylabel("Prix ($)")
    ax2.legend(loc="best", fontsize="small")

    # Camembert
    ax3 = axs[1, 0]
    all_assets = pd.concat([tableau_a, tableau_b])
    total_values = all_assets["Prix Moyen"]
    labels = all_assets["Nom"]
    ax3.pie(total_values, labels=None, colors=[colors_a.get(name, colors_b.get(name)) for name in labels], autopct='%1.1f%%')
    ax3.set_title("Répartition des actifs")

    # Liste des actifs
    ax4 = axs[1, 1]
    ax4.axis("off")
    table_data = all_assets[["Nom", "Prix Moyen"]]
    ax4.table(cellText=table_data.values, colLabels=table_data.columns, cellLoc="center", loc="center")

    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.show()

# Préparer les données
line_df, tableau_a, tableau_b = prepare_data(portfolio_data)

# Visualiser
visualize_portfolio(line_df, tableau_a, tableau_b)
