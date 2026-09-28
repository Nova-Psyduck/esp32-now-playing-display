#!/bin/bash
ESP32_IP="Your ESP IP"

echo "Listening to Brave for track changes..."

playerctl --player=brave metadata --format "{{ status }}|{{ title }} - {{ artist }}" --follow | while read -r raw_info; do

  # Split the status from the song name
  status=$(echo "$raw_info" | cut -d'|' -f1)
  track_info=$(echo "$raw_info" | cut -d'|' -f2-)

  # If the music is paused, stopped, or empty, send the STOP command
  if [[ "$status" == "Paused" || "$status" == "Stopped" || "$track_info" == " - " ]]; then
    echo "Music Paused. Turning off LED."
    curl -s "http://$ESP32_IP/update" -H "Content-Type: text/plain" --data-raw "STOP" >/dev/null
    continue
  fi

  echo "Sending to OLED: $track_info"
  curl -s "http://$ESP32_IP/update" -H "Content-Type: text/plain" --data-raw "$track_info" >/dev/null

  sleep 1
done
