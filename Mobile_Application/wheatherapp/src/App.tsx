import {
  IonApp,
  IonHeader,
  IonContent,
  IonTitle,
  IonToolbar,
  IonPage,
  IonText,
  IonButton,
} from '@ionic/react';

import React, { useEffect, useState } from 'react';

import { setupIonicReact } from '@ionic/react';

/* CSS Ionic */
import '@ionic/react/css/core.css';
import '@ionic/react/css/normalize.css';
import '@ionic/react/css/structure.css';
import '@ionic/react/css/typography.css';
import '@ionic/react/css/padding.css';
import '@ionic/react/css/flex-utils.css';
import '@ionic/react/css/display.css';
import '@ionic/react/css/palettes/dark.system.css';

import './theme/variables.css';

setupIonicReact();

const App: React.FC = () => {
  const [data, setData] = useState<any>(null); // Vom folosi un obiect pentru a stoca datele JSON
  const [loading, setLoading] = useState<boolean>(true);
  const [error, setError] = useState<string | null>(null);

  const fetchData = async () => {
    setLoading(true);
    setError(null);
    try {
      const response = await fetch('http://192.168.1.1/data'); //Schimba url-ul cu cel pe care ruleaza placa ta
      if (!response.ok) throw new Error('Eroare la preluarea datelor.');
      const data = await response.json(); // Parsem datele ca JSON
      setData(data);
    } catch (err: any) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchData();
  }, []);

  return (
    <IonApp>
      <IonPage>
        <IonHeader>
          <IonToolbar>
            <IonTitle>Statie Meteo</IonTitle>
          </IonToolbar>
        </IonHeader>
        <IonContent className="ion-padding">
          {loading ? (
            <IonText>Se încarcă...</IonText>
          ) : error ? (
            <IonText color="danger">{error}</IonText>
          ) : (
            <div className="weather-data-container">
              <div className="weather-item">
                <IonText color="primary">
                  <h2>Temperatura</h2>
                  <p>{data.temperature} °C</p>
                </IonText>
              </div>

              <div className="weather-item">
                <IonText color="secondary">
                  <h2>Umiditate</h2>
                  <p>{data.humidity} %</p>
                </IonText>
              </div>

              <div className="weather-item">
                <IonText color="tertiary">
                  <h2>CO2 (MQ-135)</h2>
                  <p>{data.mq135}</p>
                </IonText>
              </div>
            </div>
          )}
          <IonButton onClick={fetchData}>Reîncarcă</IonButton>
        </IonContent>
      </IonPage>
    </IonApp>
  );
};

export default App;
