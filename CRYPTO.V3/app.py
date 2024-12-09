import json
import pandas as pd
import dash
from dash import dcc, html
import plotly.graph_objs as go

def load_data(file_path):
    try:
        with open(file_path, "r") as file:
            return json.load(file)
    except Exception as e:
        print(f"Erreur lors du chargement des données : {e}")
        return None

def prepare_data(data):
    line_data = []
    for asset in data["assets"]:
        name = asset["name"]
        previous_quantity = 0
        for entry in asset["historique"]:
            current_quantity = entry["quantite"]
            action = "Achat" if current_quantity > previous_quantity else "Vente" if current_quantity < previous_quantity else "Mise à jour"
            montant = (current_quantity - previous_quantity) * entry["prix"] if action == "Achat" else 0
            line_data.append({
                "Nom": name,
                "Date": entry["date"],
                "Prix": entry["prix"],
                "Quantité": current_quantity,
                "Quantité Précédente": previous_quantity,
                "Action": action,
                "Montant": montant
            })
            previous_quantity = current_quantity
    line_df = pd.DataFrame(line_data)
    line_df["Date"] = pd.to_datetime(line_df["Date"])
    return line_df.sort_values(by="Date")

def generate_summary(line_df):
    portfolio_summary = []
    total_value = 0
    total_invested = 0

    for name, group in line_df.groupby("Nom"):
        total_quantity = 0
        invested_value = 0
        for i, row in group.iterrows():
            if row["Action"] == "Achat":
                invested_value += (row["Quantité"] - row["Quantité Précédente"]) * row["Prix"]
            total_quantity = row["Quantité"]

        last_entry = group.iloc[-1]
        current_value = last_entry["Prix"] * total_quantity
        pnl = current_value - invested_value
        pnl_percentage = (pnl / invested_value) * 100 if invested_value != 0 else 0

        total_value += current_value
        total_invested += invested_value

        portfolio_summary.append({
            "Nom": name,
            "Quantité Totale": total_quantity,
            "Prix Moyen": invested_value / total_quantity if total_quantity != 0 else 0,
            "Valeur Actuelle": current_value,
            "PNL": pnl,
            "PNL (%)": pnl_percentage,
            "Historique": group
        })

    total_pnl = total_value - total_invested
    total_pnl_percentage = (total_pnl / total_invested) * 100 if total_invested != 0 else 0

    return portfolio_summary, total_value, total_invested, total_pnl, total_pnl_percentage

def create_asset_row(asset, total_value):
    percentage = (asset['Valeur Actuelle'] / total_value) * 100

    hist_df = pd.DataFrame(asset['Historique'])
    hist_df['Date'] = pd.to_datetime(hist_df['Date'])
    hist_df = hist_df.sort_values('Date')
    hist_df = hist_df.dropna(subset=['Date', 'Prix'])

    return html.Div([
        # Colonne 1 - Informations de l'actif (15%)
        html.Div([
            html.Div([
                html.Div(style={
                    'display': 'flex',
                    'justifyContent': 'space-between',
                    'alignItems': 'center',
                    'marginBottom': '5px'
                }, children=[
                    html.H3(asset['Nom'], style={
                        'color': '#000000',
                        'fontWeight': '900',
                        'fontSize': '32px',
                        'padding': '5px 10px',
                        'border': '2px solid #000000',
                        'borderRadius': '3px',
                        'margin': '0',
                        'fontFamily': 'Neue Haas Display',
                        'backgroundColor': '#333'
                    }),
                    html.Div([
                        html.P(f"${asset['Valeur Actuelle']:.2f}", style={
                            'color': '#FFD700',
                            'fontSize': '20px',
                            'margin': '0 0 3px 0',
                            'textAlign': 'right',
                            'fontWeight': 'bold'
                        }),
                        html.P(f"{asset['PNL']:.2f}$", style={
                            'color': 'green' if asset['PNL'] >= 0 else 'red',
                            'fontSize': '20px',
                            'margin': '0 0 3px 0',
                            'textAlign': 'right',
                            'fontWeight': 'bold'
                        }),
                        html.P(f"{asset['PNL (%)']:.2f}%", style={
                            'color': 'green' if asset['PNL (%)'] >= 0 else 'red',
                            'fontSize': '20px',
                            'margin': '0',
                            'textAlign': 'right',
                            'fontWeight': 'bold'
                        })
                    ])
                ]),
                html.Div(style={'borderTop': '1px solid #444', 'margin': '10px 0'}),
                html.P(f"Quantité: {asset['Quantité Totale']:.2f}",
                      style={'color': '#f9f9f9', 'margin': '3px 0', 'fontSize': '16px'}),
                html.P(f"Prix Moyen: {asset['Prix Moyen']:.4f}$",
                      style={'color': '#f9f9f9', 'margin': '3px 0', 'fontSize': '16px'})
            ])
        ], style={'width': '15%', 'padding': '5px'}),

        # Colonne 2 - Historique des prix (40%)
        html.Div([
            html.Table([
                html.Thead(
                    html.Tr([
                        html.Th("Date", style={'color': '#f9f9f9', 'padding': '3px 10px', 'textAlign': 'left'}),
                        html.Th("Prix", style={'color': '#f9f9f9', 'padding': '3px 10px', 'textAlign': 'left'}),
                        html.Th("Action", style={'color': '#f9f9f9', 'padding': '3px 10px', 'textAlign': 'left'}),
                        html.Th("Détails", style={'color': '#f9f9f9', 'padding': '3px 10px', 'textAlign': 'left'})
                    ])
                ),
                html.Tbody([
                    html.Tr([
                        html.Td(f"[{row['Date'].strftime('%d-%m-%Y')}]",
                               style={'color': '#FFD700', 'padding': '3px 10px', 'textAlign': 'left'}),
                        html.Td(f"${row['Prix']:.4f}",
                               style={'color': '#87CEEB', 'padding': '3px 10px', 'textAlign': 'left'}),
                        html.Td(row['Action'],
                               style={'color': '#FFA07A', 'padding': '3px 10px', 'textAlign': 'left'}),
                        html.Td(
                            f"(+{row['Quantité'] - row['Quantité Précédente']:.2f} = ${row['Montant']:.2f})" if row['Action'] == "Achat" else "",
                            style={'color': '#808080', 'padding': '3px 10px', 'textAlign': 'left', 'fontStyle': 'italic'}
                        )
                    ]) for _, row in hist_df.iterrows()
                ])
            ], style={'width': '100%', 'borderCollapse': 'collapse'})
        ], style={'width': '40%', 'padding': '5px'}),

        # Colonne 3 - Pourcentage du portefeuille (15%)
        html.Div([
            dcc.Graph(
                figure=go.Figure(
                    data=[go.Pie(
                        labels=[asset['Nom'], 'Autres'],
                        values=[percentage, 100-percentage],
                        hole=.3,
                        type='pie',
                        textinfo='percent',
                        showlegend=False
                    )],
                    layout=go.Layout(
                        margin={'l': 0, 'r': 0, 't': 0, 'b': 0},
                        plot_bgcolor='#2c2c2c',
                        paper_bgcolor='#2c2c2c',
                        font={'color': '#f9f9f9'},
                        showlegend=False
                    )
                ),
                style={'height': '140px'},
                config={'displayModeBar': False}
            )
        ], style={'width': '15%', 'padding': '0'}),

        # Colonne 4 - Graphique d'évolution des prix (30%)
        html.Div([
            dcc.Graph(
                figure=go.Figure(
                    data=[
                        go.Scatter(
                            x=hist_df['Date'],
                            y=hist_df['Prix'],
                            mode='lines+markers',
                            name=asset['Nom'],
                            line=dict(
                                width=2,
                                shape='linear',
                                smoothing=0
                            ),
                            marker=dict(
                                size=6,
                                line=dict(width=0)
                            ),
                            hoverinfo='x+y',
                            connectgaps=True
                        )
                    ],
                    layout=go.Layout(
                        plot_bgcolor='#2c2c2c',
                        paper_bgcolor='#2c2c2c',
                        font={'color': '#f9f9f9'},
                        margin=dict(l=0, r=0, t=0, b=0),
                        xaxis=dict(
                            showgrid=True,
                            gridcolor='#444',
                            title=None,
                            tickformat='%b-%y',
                            showline=False,
                            zeroline=False
                        ),
                        yaxis=dict(
                            showgrid=True,
                            gridcolor='#444',
                            title=None,
                            showline=False,
                            zeroline=False
                        ),
                        showlegend=False
                    )
                ),
                style={'height': '140px'},
                config={'displayModeBar': False}
            )
        ], style={'width': '30%', 'padding': '0'})
    ], style={
        'display': 'flex',
        'alignItems': 'center',
        'padding': '8px',
        'marginBottom': '8px',
        'backgroundColor': '#333',
        'borderRadius': '5px',
        'border': '1px solid #444'
    })

# Charger et préparer les données
portfolio_data = load_data("MAIN.txt")
if portfolio_data is None:
    print("Erreur : Impossible de charger les données.")
    exit()

line_df = prepare_data(portfolio_data)
portfolio_summary, total_value, total_invested, total_pnl, total_pnl_percentage = generate_summary(line_df)

sorted_assets = sorted(portfolio_summary,
                      key=lambda x: x['Valeur Actuelle'] / total_value,
                      reverse=True)

app = dash.Dash(__name__)

app.layout = html.Div(style={
    'backgroundColor': '#2c2c2c',
    'padding': '20px',
    'fontFamily': 'Neue Haas Display, sans-serif'
}, children=[
    # En-tête du portefeuille
    html.Div([
        html.H1("Résumé du Portefeuille",
                style={'textAlign': 'center', 'color': '#f9f9f9', 'fontWeight': 'bold'}),
        html.Div([
            html.Div(f"Capital total investi : ${total_invested:,.2f}",
                     style={'color': '#FFD700', 'fontSize': '18px', 'marginBottom': '5px'}),
            html.Div(f"Valeur actuelle : ${total_value:,.2f}",
                     style={'color': '#87CEEB', 'fontSize': '18px', 'marginBottom': '5px'}),
            html.Div(f"PNL : ${total_pnl:,.2f} ({total_pnl_percentage:.2f}%)",
                     style={
                         'color': 'green' if total_pnl >= 0 else 'red',
                         'fontSize': '18px',
                         'marginBottom': '5px'
                     })
        ], style={'textAlign': 'center', 'marginBottom': '20px'})
    ]),

    # Liste des actifs
    html.Div([create_asset_row(asset, total_value) for asset in sorted_assets])
])

if __name__ == '__main__':
    print("Lancement du serveur Dash...")
    app.run_server(debug=True)
