/**
 * Módulo encargado del Canvas de Usuario. Permite dibujo libre y parametrización del dibujo
 */
const UserCanvas = {
    w: 500, // El ancho de este canvas
    DIM: 10, // resolution. Mínima distancia entre dos puntos consecutivos
    canvas: null, ctx: null,
    points: [], // Puntos del dibujo
    lastPoint: null,
    down: false, // Si el usuario está dibujando
    relation: .3, // Relación entre un punto real en cm y un píxel de este canvas
    mouse: {x: 0, y: 0},

    setDimensions(width, height) {
        this.relation = width / this.w
        this.h = height / this.relation

        this.canvas.width = this.w
        this.canvas.height = this.h
        this.canvas.style.width = this.w + "px"
        this.canvas.style.height = this.h + "px"

        this.width = width
        this.height = height

        this.draw()
    },

    init () {
        this.w = Math.min(500, window.innerWidth - 40)
        this.canvas = document.getElementById("canvas")
        this.ctx = this.canvas.getContext("2d")

        this.relation = base / this.w
        this.h = altura / this.relation

        this.width = base
        this.height = altura

        this.canvas.width = this.w
        this.canvas.height = this.h
        this.canvas.style.width = this.w + "px"
        this.canvas.style.height = this.h + "px"

        this.canvas.addEventListener("mousedown", e => this.mousedown(e))
        this.canvas.addEventListener("mouseup",   e => this.mouseup(e))
        this.canvas.addEventListener("mousemove", e => this.mousemove(e))

        this.canvas.addEventListener("touchstart", e => this.mousedown(e.touches[0]))
        this.canvas.addEventListener("touchmove",  e => {this.mousemove(e.touches[0]); e.preventDefault()})
        this.canvas.addEventListener("touchend",   e => this.mouseup(e.changedTouches[0]))

        this.canvas.parentNode.addEventListener("fullscreenchange", e => { if(!document.fullscreenElement) this.nofullscreen()})
    },

    /**
     * Devuelve las coordenadas del punto donde está el mouse
     */
    mouseToPoint(e) {
        const cRect = this.canvas.getBoundingClientRect();        // Gets CSS pos, and width/height

        return {
            x: Math.round(e.clientX - cRect.left),
            y: Math.round(e.clientY - cRect.top)
        }
    },

    // Inicia un nuevo segmento
    mousedown(e) {
        this.lastPoint = this.mouseToPoint(e)
        this.points.push(this.lastPoint, "DOWN")
        this.down = true
        this.draw()
    },

    // Termina el segmento
    mouseup() {
        this.points.push("UP")
        this.down = false
        this.draw()
    },

    // Añade nuevos puntos al segmento cuando se mueve el mouse
    mousemove(e) {
        this.mouse = this.mouseToPoint(e)
        this.draw()
        if (!this.lastPoint || !this.down) return
        const dx = this.mouse.x - this.lastPoint.x
        const dy = this.mouse.y - this.lastPoint.y
        if (dx * dx + dy * dy > this.DIM * this.DIM) {
            this.lastPoint = this.mouse
            this.points.push(this.lastPoint)
        }
    },

    // Dibuja los segmentos en el canvas, cada uno con un color distinto
    draw() {
        count.innerText = this.points.filter(x => x !== "DOWN" && x !== "UP").length
        this.ctx.clearRect(0, 0, this.w, this.h);

        this.ctx.fillStyle = "#eee";
        const point = this.down ? this.lastPoint : this.mouse
        this.ctx.beginPath()
            this.ctx.arc(point.x, point.y, this.DIM, 0, 2 * Math.PI, false)
        this.ctx.fill()

        let hue = 0
        let down = false
        this.ctx.strokeStyle = "#000" //`hsl(${hue}, 100%, 50%)`
        this.ctx.beginPath()
        for (const p of this.points) {
            if (p === "UP") {
                this.ctx.stroke()
                this.ctx.beginPath()
                down = false
                hue = (hue + 20) % 360
                this.ctx.strokeStyle = "#000" //`hsl(${hue}, 100%, 50%)`
            } else if (p === "DOWN") {
                down = true
            } else {
                if (down) {
                    this.ctx.lineTo(p.x, p.y)
                } else {
                    this.ctx.moveTo(p.x, p.y)
                }
            }
        }
        this.ctx.stroke()
    },

    // Transforma un punto del canvas en un punto real en cm
    pointToReal(point) {
        return {
            x: this.width - point.y * this.relation + minRadius,
            y: this.height * .5 - point.x * this.relation
        }
    },

    // Recorta los dígitos decimales del valor a 4
    truncatePoint(p) {
        p.x = parseFloat(p.x.toFixed(4))
        p.y = parseFloat(p.y.toFixed(4))
    },

    // Transforma la serie de puntos en una cadena de texto
    // Esta cadena es la que se debe transmitir al MCU por medio de un HTTP Request
    toString() {
        let t = ""
        for (const p of this.points) {
            if (p === "UP" || p === "DOWN")
                t += p + ","
            else {
                const real = this.pointToReal(p)
                t += real.x.toFixed(4) + "," + real.y.toFixed(4) + ","
            }
        }
        return t
    },

    clear() {
        this.points.length = 0
        this.draw()
    },

    eraseLast() {
        this.points.splice(this.points.lastIndexOf("DOWN") - 1)
        this.draw()
    },

    fullscreen() {
        document.getElementById('canvas_body').requestFullscreen()
        this.w = window.innerWidth - 40
        this.setDimensions(this.width, this.height)
    },

    nofullscreen() {
        this.w = Math.min(500, window.innerWidth - 40)
        this.setDimensions(this.width, this.height)
    },
}
