/**
 *  ST_Door_Lock.groovy
 *
 *  Copyright 2015 Josh Villbrandt
 */

metadata {
  definition (name: "ST_Door_Lock", namespace: "jvillbrandt", author: "Josh Villbrandt") {
    capability "Motion Sensor"
    capability "Contact Sensor"
  }

  simulator {
  }

  // Preferences
  preferences {
  }

  // Tile Definitions
  tiles {
    standardTile("lock", "device.lock", width: 1, height: 1) {
      state("open", label:'${name}', icon:"st.locks.lock.unlocked", backgroundColor:"#ffa81e")
      state("closed", label:'${name}', icon:"st.locks.lock.locked", backgroundColor:"#79b821")
    }

    standardTile("door", "device.door", width: 1, height: 1) {
      state("open", label:'${name}', icon:"st.contact.contact.open", backgroundColor:"#ffa81e")
      state("closed", label:'${name}', icon:"st.contact.contact.closed", backgroundColor:"#79b821")
    }

    standardTile("motion", "device.motion", width: 1, height: 1) {
      state("active", label:'motion', icon:"st.motion.motion.active", backgroundColor:"#53a7c0")
      state("inactive", label:'no motion', icon:"st.motion.motion.inactive", backgroundColor:"#ffffff")
    }

    main(["lock","door","motion"])
    details(["lock","door","motion"])
  }
}

// parse events into attributes
def parse(String description) {
  log.debug "Parsing '${description}'"
    def msg = zigbee.parse(description)?.text
    log.debug "Parse got '${msg}'"

    def parts = msg.split(" ")
    def name  = parts.length>0?parts[0].trim():null
    def value = parts.length>1?parts[1].trim():null

    name = value != "ping" ? name : null

    def result = createEvent(name: name, value: value)

    log.debug result

    return result
}

// handle commands
def poll() {
}
def configure() {
}
