include('lib/server/http_server')
include('lib/message_queue/client')

const error_404 = (resp) => {
    resp.set_status(404).set_data('<h1>404</h1>').end()
}

const ok_200 = (resp, path) => {
    resp.set_status(200).set_data(f('<pre>{}</pre>', fs.read(path))).end()
}

const global_store = {
    buf: []
}

const message_queue = create_message_queue_client(10010)

start_timer(() => {
    message_queue.shift().then((data) => {
        (data != null) ? @{
            log('received a message', data)
            global_store.buf.push(data)
        }: null
    })
}, 1000)

const blue_tag = log_color.blue('[log]')
make_http_server(8899, {
    on_init: server => {
        const url = f('http://127.0.0.1:{}?path=http_server.as', server.port)
        f_log('{} 服务器启动等待连接 url:{}', blue_tag, url)
        shell(f('open {}', url))
    },
    on_message: (req, resp) => {
        resp.header.set('Server', 'Agumi').set('Content-Type', 'text/html; charset=utf-8')
        const path_req = req.params.path
        f_log('{} 接受到新请求 path:{}', blue_tag, path_req)
        const has_path = () => {
            const path = path_calc(env().curr_dir(), path_req)
            (fs.exist(path)) ? ok_200(resp, path) : error_404(resp)
        }
        path_req ? has_path() : error_404(resp)
    }
})

//include('script/http_server.as', true)
//include('script/index.as', true)
