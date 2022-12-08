const express = require("express")
const bodyParser = require("body-parser")
const app = express()
const axios = require("axios")

let robotIp = null

// Configuramos express para usar a body-parser como middleware
app.use(bodyParser.urlencoded({ extended: true }))

app.listen(3000, () => {
    console.log("Aplicación iniciada y escuchando en puerto 3000")
})

// para las css estáticas
app.use(express.static(__dirname))

app.get("/", (req, res) => {
    res.sendFile(__dirname + "/index.html")
})

app.post("/connect", (req, res) => {
    if (req.body) {
        robotIp = req.body.ip
        axios({
            method: "get",
            url: `http://${robotIp}/connect`,
            timeout: 5000
        })
            .then(r => {
                console.log("Connect: " + r.status)
                res.end("connected")
            })
            .catch(error => res.end(error.code))
    } else {
        res.end("Falta IP")
    }
})

app.post("/move", (req, res) => {
    axios
        .post(`http://${robotIp}/m${req.body.q}${req.body.d}`)
        .then(res => {
            console.log(`Status: ${res.status})`)
        })
        .catch(err => {
            console.error(err)
        })
})

app.post("/draw", (req, res) => {
    console.log("Sending: [" + req.body.q + "] " + req.body.ps)
    axios
        .get(`http://${robotIp}/draw`, {params: {q: req.body.q, ps: req.body.ps}})
        .then(res_a => {
            console.log(`Status: ${res_a.status}`)
            res.end("end_draw")
        })
        .catch(err => {
            console.error(err)
        })
})
